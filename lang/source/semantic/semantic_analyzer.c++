#include "aw/script/semantic/semantic_analyzer.h"

#include "context.h"

#include <aw/script/semantic/scope.h>

#include <aw/script/diag/error_t.h>

#include <aw/utility/ranges/paired.h>

namespace aw::script {

template<typename Value, typename T = std::remove_reference_t<Value>>
auto wrap(Value&& val) -> std::unique_ptr<T>
{
	return std::make_unique<T>(std::forward<Value>(val));
}

auto create_type(std::string_view name) -> std::unique_ptr<ast::type>
{
	ast::type type;
	type.name = std::string(name);
	return wrap(std::move(type));
}

static auto create_builtin_types()
{
	std::vector<std::unique_ptr<ast::type>> types;

	types.push_back(create_type("void"));

	// TODO: aliases
	types.push_back(create_type("int"));
	types.push_back(create_type("i32"));
	types.push_back(create_type("i64"));

	//top_scope->add_symbol("uint", create_type("uint"));
	//top_scope->add_symbol("u32", create_type("u32"));
	//top_scope->add_symbol("u64", create_type("u64"));

	types.push_back(create_type("float"));
	types.push_back(create_type("double"));
	types.push_back(create_type("f32"));
	types.push_back(create_type("f64"));

	types.push_back(create_type("string_literal"));

	// TODO: remove when FFI modules are implemented
	types.push_back(create_type("cstring"));

	return types;
}

semantic_analyzer::semantic_analyzer()
	: builtin_types(create_builtin_types())
{
}


auto semantic_analyzer::lower(const ast::module& in_mod) -> middle::module
{
	context ctx;

	for (const auto& type : builtin_types)
	{
		ctx.default_scope.add_symbol(type->name, type.get());
	}

	ctx.push_scope();

	middle::module mod;

	for (const auto& in_decl : in_mod)
	{
		auto decl_ptr = wrap(visit(ctx, in_decl));
		if (auto decl = std::get_if<middle::variable>(decl_ptr.get()))
			ctx.current_scope()->add_symbol(decl->name, decl);
		if (auto decl = std::get_if<middle::function>(decl_ptr.get()))
			ctx.current_scope()->add_symbol(decl->name, decl);
		mod.push_back(std::move(decl_ptr));
	}

	return mod;
}

/*
 * Declarations
 */
auto semantic_analyzer::visit(context& ctx, const ast::declaration& decl) -> middle::declaration
{
	auto ast_visitor = [this, &ctx] (const auto& decl) -> middle::declaration
	{
		return visit(ctx, decl);
	};

	return std::visit(ast_visitor, decl);
}

auto semantic_analyzer::visit(context& ctx, const ast::function& in_func) -> middle::function
{
	middle::function func;
	func.name = in_func.name;
	func.args = visit(ctx, in_func.args);
	func.return_type = ctx.find_type(in_func.return_type);

	if (in_func.body) {
		// TODO: local ctx instead of push/pop scope
		ctx.push_scope();
		ctx.current_scope()->add_symbol(func.name, &func);
		func.body = visit_stmt(ctx, *in_func.body);
		ctx.pop_scope();
	}
	return func;
}

auto semantic_analyzer::visit(context& ctx, const ast::parameter_list& in_list) -> middle::parameter_list
{
	middle::parameter_list result;
	result.reserve(in_list.size());
	for (const auto& var : in_list)
	{
		result.push_back(wrap(visit(ctx, var)));
		ctx.current_scope()->add_symbol(var.name, result.back().get());
	}
	return result;
}

auto semantic_analyzer::visit(context& ctx, const ast::variable& in_var) -> middle::variable
{
	middle::variable var;
	var.name = in_var.name;
	var.access = in_var.access;
	if (in_var.value)
		var.value = wrap(visit_expr(ctx, *in_var.value));

	if (!in_var.type.empty())
		var.type = ctx.find_type(in_var.type);
	return var;
}

/*
 * Statements
 */
auto semantic_analyzer::visit_stmt(context& ctx, const ast::statement& in_stmt) -> middle::statement
{
	auto stmt_visitor = [this, &ctx] (const auto& stmt) -> middle::statement
	{
		return visit_stmt(ctx, stmt);
	};
	return std::visit(stmt_visitor, in_stmt);
}

auto semantic_analyzer::visit_stmt(context& ctx, const ast::statement_block& in_block) -> middle::statement_block
{
	middle::statement_block block;
	for (const auto& stmt : in_block)
	{
		block.push_back(visit_stmt(ctx, stmt));
	}
	return block;

}
auto semantic_analyzer::visit_stmt(context& ctx, const ast::if_else_statement& in_stmt) -> middle::if_else_statement
{
	middle::if_else_statement stmt;

	if (in_stmt.if_expr)
		stmt.if_expr = wrap(visit_expr(ctx, *in_stmt.if_expr));
	if (in_stmt.if_body)
		stmt.if_body = wrap(visit_stmt(ctx, *in_stmt.if_body));
	if (in_stmt.else_body)
		stmt.else_body = wrap(visit_stmt(ctx, *in_stmt.else_body));
	return stmt;
}

auto semantic_analyzer::visit_stmt(context& ctx, const ast::return_statement& in_stmt) -> middle::return_statement
{
	middle::return_statement stmt;
	if (in_stmt.value)
	{
		auto expr = visit_expr(ctx, *in_stmt.value);

		stmt.value = wrap(std::move(expr));
	}
	return stmt;
}

auto semantic_analyzer::visit_stmt(context& ctx, const ast::empty_statement& /*in_stmt*/) -> middle::empty_statement
{
	return middle::empty_statement{};
}

auto semantic_analyzer::visit_stmt(context& ctx, const ast::expression& in_expr) -> middle::expression
{
	auto expr = visit_expr(ctx, in_expr);
	return expr;
}

/*
 * Expressions
 */
auto semantic_analyzer::visit_expr(context& ctx, const ast::expression& in_expr) -> middle::expression
{
	auto expr_visitor = [this, &ctx] (const auto& expr) -> middle::expression
	{
		return visit_expr(ctx, expr);
	};
	return std::visit(expr_visitor, in_expr);
}


auto convert_operator(ast::unary_operator op) -> middle::unary_operator
{
	using enum ast::unary_operator;

	using unop = middle::unary_operator;
	switch(op) {
	case minus:
		return unop::minus;
	case plus:
		return unop::plus;
	case logical_negation:
	case binary_negation:
		return unop::negation;
	}
};

auto convert_operator(ast::binary_operator op) -> middle::binary_operator
{
	using enum ast::binary_operator;

	using binop = middle::binary_operator;
	switch(op) {
	case minus:
		return binop::minus;
	case plus:
		return binop::plus;
	case multiply:
		return binop::multiply;
	case divide:
		return binop::divide;
	case less:
		return binop::less;
	case greater:
		return binop::greater;
	case equal:
		return binop::equal;
	case not_equal:
		return binop::not_equal;
	case assignment:
		return binop::assignment;
	}
};


auto semantic_analyzer::visit_expr(context& ctx, const ast::unary_expression& in_expr) -> middle::unary_expression
{
	middle::unary_expression expr;

	expr.op = convert_operator(in_expr.op);
	expr.lhs = wrap(visit_expr(ctx, *in_expr.lhs));

	return expr;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::binary_expression& in_expr) -> middle::binary_expression
{
	middle::binary_expression expr;

	expr.lhs = wrap(visit_expr(ctx, *in_expr.lhs));
	expr.op = convert_operator(in_expr.op);
	expr.rhs = wrap(visit_expr(ctx, *in_expr.rhs));

	return expr;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::call_expression& in_expr) -> middle::call_expression
{
	middle::call_expression call;
	call.func_name = in_expr.func;
	for (const auto& arg : in_expr.args)
	{
		call.args.push_back(visit_expr(ctx, arg));
	}
	call.func = ctx.current_scope()->find_func(call.func_name);
	return call;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::if_expression& in_expr) -> middle::if_expression
{
	middle::if_expression expr;
	if (in_expr.if_expr)
		expr.if_expr = wrap(visit_expr(ctx, *in_expr.if_expr));
	if (in_expr.if_body)
		expr.if_body = wrap(visit_expr(ctx, *in_expr.if_body));
	if (in_expr.else_body)
		expr.else_body = wrap(visit_expr(ctx, *in_expr.else_body));
	return expr;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::value_expression& in_expr) -> middle::value_expression
{
	middle::value_expression expr;
	expr.name = in_expr.name;
	expr.ref = ctx.current_scope()->find_var(expr.name);
	return expr;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::numeric_literal& in_expr) -> middle::numeric_literal
{
	middle::numeric_literal expr;
	expr.base = in_expr.base;
	expr.value = in_expr.value;
	return expr;
}

auto semantic_analyzer::visit_expr(context& ctx, const ast::string_literal& in_expr) -> middle::string_literal
{
	middle::string_literal expr;
	expr.value = in_expr.value;
	return expr;
}
