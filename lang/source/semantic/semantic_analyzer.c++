#include "aw/script/semantic/semantic_analyzer.h"

#include "context.h"
#include "convert_to_middle.h"

#include <aw/script/semantic/scope.h>
#include <aw/script/utility/wrap.h>

#include <aw/script/diag/error_t.h>

#include <aw/utility/ranges/paired.h>

namespace aw::script {
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
	types.push_back(create_type("bool"));

	// TODO: aliases
	types.push_back(create_type("int"));
	types.push_back(create_type("i32"));
	types.push_back(create_type("i64"));

	//top_scope->add_symbol("uint", create_type("uint"));
	//top_scope->add_symbol("u32", create_type("u32"));
	//top_scope->add_symbol("u64", create_type("u64"));

	types.push_back(wrap(ast::type{"float", ast::fp_type{ 32 }}));
	types.push_back(wrap(ast::type{"double", ast::fp_type{ 64 }}));
	types.push_back(wrap(ast::type{"f32", ast::fp_type{ 32 }}));
	types.push_back(wrap(ast::type{"f64", ast::fp_type{ 64 }}));

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
		ctx.default_scope.add_type(type->name, type.get());
	}


	middle::module mod = convert_to_middle(ctx, in_mod);

	ctx.push_scope();
	for (const auto& decl_ptr : mod)
	{
		if (auto decl = std::get_if<middle::variable>(decl_ptr.get()))
			ctx.current_scope()->add_symbol(decl->name, decl);
		if (auto decl = std::get_if<middle::function>(decl_ptr.get()))
			ctx.current_scope()->add_symbol(decl->name, decl);
	}

	for (const auto& decl_ptr : mod)
	{
		visit(ctx, *decl_ptr);
	}

	return mod;
}

/*
 * Declarations
 */
void semantic_analyzer::visit(context& ctx, middle::declaration& decl)
{
	auto ast_visitor = [this, &ctx] (auto& decl)
	{
		return visit(ctx, decl);
	};

	return std::visit(ast_visitor, decl);
}

void semantic_analyzer::visit(context& ctx, middle::function& func)
{
	for (auto& var : func.args)
	{
		visit(ctx, *var);
	}

	if (func.body) {
		// TODO: local ctx instead of push/pop scope
		ctx.push_scope();
		ctx.current_scope()->add_symbol("$func", &func); // hack!
		visit_stmt(ctx, *func.body);
		ctx.pop_scope();
	}
}

void semantic_analyzer::visit(context& ctx, middle::variable& var)
{
	ctx.current_scope()->add_symbol(var.name, &var);
	if (var.value) {
		visit_expr(ctx, *var.value);
		if (!var.type)
			var.type = infer_type(ctx, *var.value);
		else
			propagate_type(ctx, var.type, *var.value);
	}
}

/*
 * Statements
 */
void semantic_analyzer::visit_stmt(context& ctx, middle::statement& in_stmt)
{
	auto stmt_visitor = [this, &ctx] (auto& stmt)
	{
		return visit_stmt(ctx, stmt);
	};
	std::visit(stmt_visitor, in_stmt);
}

void semantic_analyzer::visit_stmt(context& ctx, middle::statement_block& in_block)
{
	ctx.push_scope();
	for (auto& stmt : in_block)
	{
		visit_stmt(ctx, stmt);
	}
	ctx.pop_scope();
}

void semantic_analyzer::visit_stmt(context& ctx, middle::if_else_statement& stmt)
{
	if (stmt.if_expr) {
		visit_expr(ctx, *stmt.if_expr);
		propagate_type(ctx, ctx.find_type("bool"), *stmt.if_expr);
	}
	if (stmt.if_body)
		visit_stmt(ctx, *stmt.if_body);
	if (stmt.else_body)
		visit_stmt(ctx, *stmt.else_body);
}

void semantic_analyzer::visit_stmt(context& ctx, middle::return_statement& stmt)
{
	if (stmt.value)
	{
		visit_expr(ctx, *stmt.value);

		if (auto func = ctx.current_scope()->find_func("$func")) // hack!
			propagate_type(ctx, func->return_type, *stmt.value);
	}
}

void semantic_analyzer::visit_stmt(context& ctx, middle::empty_statement& /*stmt*/)
{
}

void semantic_analyzer::visit_stmt(context& ctx, middle::expression& expr)
{
	visit_expr(ctx, expr);
	if (expr.type)
		propagate_type(ctx, expr.type, expr);
	else
		expr.type = infer_type(ctx, expr);
}

/*
 * Expressions
 */
void semantic_analyzer::visit_expr(context& ctx, middle::expression& expr)
{
	auto expr_visitor = [this, &ctx] (auto& expr)
	{
		return visit_expr(ctx, expr);
	};
	return std::visit(expr_visitor, expr);
}

void semantic_analyzer::visit_expr(context& ctx, middle::unary_expression& in_expr)
{
	visit_expr(ctx, *in_expr.lhs);
}

void semantic_analyzer::visit_expr(context& ctx, middle::binary_expression& in_expr)
{
	visit_expr(ctx, *in_expr.lhs);
	visit_expr(ctx, *in_expr.rhs);
}

void semantic_analyzer::visit_expr(context& ctx, middle::call_expression& call)
{
	for (auto& arg : call.args)
	{
		visit_expr(ctx, arg);
	}
	call.func = ctx.current_scope()->find_func(call.func_name);
}

void semantic_analyzer::visit_expr(context& ctx, middle::if_expression& in_expr)
{
	if (in_expr.if_expr)
		visit_expr(ctx, *in_expr.if_expr);
	if (in_expr.if_body)
		visit_expr(ctx, *in_expr.if_body);
	if (in_expr.else_body)
		visit_expr(ctx, *in_expr.else_body);
}

void semantic_analyzer::visit_expr(context& ctx, middle::value_expression& expr)
{
	expr.ref = ctx.current_scope()->find_var(expr.name);
}

void semantic_analyzer::visit_expr(context& ctx, middle::numeric_literal& in_expr)
{
}

void semantic_analyzer::visit_expr(context& ctx, middle::string_literal& in_expr)
{
}

void semantic_analyzer::visit_op(context& ctx, ast::type* ty, middle::binary_expression& expr)
{
	if (auto* fp = std::get_if<ast::fp_type>(&ty->kind))
	{
		using enum ir::binary_operator;
		switch(expr.op) {
		case less:
			expr.op = less_fp;
			break;
		case greater:
			expr.op = greater_fp;
			break;
		case divide:
			expr.op = divide_fp;
			break;
		default:
			break;
		}
	}
}

/*
 * Type inference
 */
auto semantic_analyzer::common_type(ast::type* a, ast::type* b) -> ast::type*
{
	if (!a || !b)
		return nullptr;

	if (a != b)
		return error_t(); // TODO

	return a;
}

auto semantic_analyzer::common_type(context& ctx, middle::expression& lhs, middle::expression& rhs) -> ast::type*
{
	auto* lhs_type = infer_type(ctx, lhs);
	auto* rhs_type = infer_type(ctx, rhs);

	if (!lhs_type && rhs_type)
		lhs_type = propagate_type(ctx, rhs_type, lhs);
	else if (lhs_type && !rhs_type)
		rhs_type = propagate_type(ctx, lhs_type, rhs);

	return common_type(lhs_type, rhs_type);
}

auto semantic_analyzer::common_type(context& ctx, ast::type* type, middle::expression& lhs, middle::expression& rhs) -> ast::type*
{
	auto* lhs_type = infer_type(ctx, lhs);
	auto* rhs_type = infer_type(ctx, rhs);

	if (!lhs_type || !rhs_type) {
		if (rhs_type)
			lhs_type = propagate_type(ctx, rhs_type, lhs);
		else if (lhs_type)
			rhs_type = propagate_type(ctx, lhs_type, rhs);
		else {
			lhs_type = propagate_type(ctx, type, lhs);
			lhs_type = propagate_type(ctx, type, rhs);
		}
	}

	return common_type(lhs_type, rhs_type);
}


auto semantic_analyzer::infer_type(context& ctx, middle::expression& expr) -> ast::type*
{
	auto expr_visitor = [this, &ctx] (auto& expr) -> ast::type*
	{
		return infer_type(ctx, expr);
	};

	expr.type = std::visit(expr_visitor, expr);

	return expr.type;
}

auto semantic_analyzer::infer_type(context& ctx, middle::unary_expression& expr) -> ast::type*
{
	return infer_type(ctx, *expr.lhs);
}

auto semantic_analyzer::infer_type(context& ctx, middle::binary_expression& expr) -> ast::type*
{
	auto ty = common_type(ctx, *expr.lhs, *expr.rhs);

	// TODO: move out of here
	visit_op(ctx, ty, expr);

	return ty;
}

auto semantic_analyzer::infer_type(context& ctx, middle::call_expression& expr) -> ast::type*
{
	if (expr.func)
	{
		auto& params = expr.func->args;

		for (const auto& [expr, param] : aw::paired(expr.args, params))
			propagate_type(ctx, param->type, expr);
		return expr.func->return_type;
	}
	return nullptr;
}

auto semantic_analyzer::infer_type(context& ctx, middle::if_expression& expr) -> ast::type*
{
	if (!expr.if_body || !expr.else_body)
		return ctx.find_type("void");

	return common_type(ctx, *expr.if_body, *expr.else_body);
}

auto semantic_analyzer::infer_type(context& ctx, middle::value_expression& expr) -> ast::type*
{
	if (auto var = expr.ref)
	{
		return var->type;
	}
	return nullptr;
}
auto semantic_analyzer::infer_type(context& ctx, middle::numeric_literal& expr) -> ast::type*
{
	return expr.type;
}
auto semantic_analyzer::infer_type(context& ctx, middle::string_literal& expr) -> ast::type*
{
	return ctx.find_type("string_literal");
}

/*
 * Type propagation
 */
auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::expression& expr) -> ast::type*
{
	auto expr_visitor = [this, type, &ctx] (auto& expr) -> ast::type*
	{
		return propagate_type(ctx, type, expr);
	};

	expr.type = std::visit(expr_visitor, expr);

	return expr.type;
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::unary_expression& expr) -> ast::type*
{
	return propagate_type(ctx, type, *expr.lhs);
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::binary_expression& expr) -> ast::type*
{
	auto ty = common_type(ctx, type, *expr.lhs, *expr.rhs);

	// TODO: move out of here
	visit_op(ctx, ty, expr);

	return ty;
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::call_expression& expr) -> ast::type*
{
	return infer_type(ctx, expr);
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::if_expression& expr) -> ast::type*
{
	propagate_type(ctx, ctx.find_type("bool"), *expr.if_expr);

	if (!expr.else_body)
	{
		propagate_type(ctx, type, *expr.if_body);
		return ctx.find_type("void");
	}

	return common_type(ctx, type, *expr.if_body, *expr.else_body);
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::value_expression& expr) -> ast::type*
{
	// TODO: if the referred-to variable has no type, it should be back-propagated
	return common_type(type, infer_type(ctx, expr));
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::numeric_literal& expr) -> ast::type*
{
	if (!expr.type)
		expr.type = type;
	return common_type(expr.type, type);
}

auto semantic_analyzer::propagate_type(context& ctx, ast::type* type, middle::string_literal& expr) -> ast::type*
{
	return common_type(type, ctx.find_type("cstring")); // TODO: should be string_literal
}

} // namespace aw::script
