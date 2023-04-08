#include "convert_to_middle.h"

#include <aw/script/utility/wrap.h>

#include <aw/utility/ranges/paired.h>

namespace aw::script {

namespace {
auto convert_operator(ast::unary_operator op) -> ir::unary_operator
{
	using unop = ir::unary_operator;
	switch(op) {
		using enum ast::unary_operator;
	case minus:
		return unop::minus;
	case plus:
		return unop::plus;
	case logical_negation:
	case binary_negation:
		return unop::negation;
	}
	assert(!"Corrupt value!");
};

auto convert_operator(ast::binary_operator op) -> ir::binary_operator
{
	using binop = ir::binary_operator;
	switch(op) {
		using enum ast::binary_operator;
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
	assert(!"Corrupt value!");
};

struct convert_to_middle_visitor {
	context& ctx;

	auto convert_decl(const ast::declaration& in_decl) -> middle::declaration
	{
		auto wrapper = [this] (const auto& decl) -> middle::declaration
		{
			return convert_decl(decl);
		};

		return std::visit(wrapper, in_decl);
	}

	auto convert_decl(const ast::struct_decl& in_struct) -> middle::struct_decl
	{
		middle::struct_decl st;
		st.name = std::string(in_struct.name);
		for (const auto& var : in_struct.members)
			st.members.push_back(wrap(convert_decl(var)));
		return st;
	}

	auto convert_decl(const ast::variable& in_var) -> middle::variable
	{
		middle::variable var;
		var.name = in_var.name;
		var.access = in_var.access;
		if (!get_if<ast::unknown_type>(&in_var.type))
			var.type = ctx.create_type(in_var.type);
		if (in_var.value)
			var.value = wrap(convert_expr(*in_var.value));

		return var;
	}

	auto convert_decl(const ast::function& in_func) -> middle::function
	{
		middle::function func;
		func.name = in_func.name;
		func.return_type = ctx.create_type(in_func.return_type);
		if (in_func.body)
			func.body = wrap(convert_stmt(*in_func.body));

		func.args.resize(in_func.params.size());
		for (auto&& [arg, in_arg] : paired(func.args, in_func.params))
		{
			arg = wrap(convert_decl(in_arg));
		}
		func.is_variadic = in_func.is_variadic();
		return func;
	}

	/*
	 * Statements
	 */
	auto convert_stmt(const ast::statement& in_stmt) -> middle::statement
	{
		auto stmt_visitor = [this] (const auto& stmt) -> middle::statement
		{
			return convert_stmt(stmt);
		};
		return std::visit(stmt_visitor, in_stmt);
	}

	auto convert_stmt(const ast::decl_statement& in_stmt) -> middle::decl_statement
	{
		return { convert_decl(*in_stmt.decl) };
	}

	auto convert_stmt(const ast::statement_block& in_block) -> middle::statement_block
	{
		middle::statement_block block;
		for (const auto& stmt : in_block)
		{
			block.push_back(convert_stmt(stmt));
		}
		return block;

	}

	auto convert_stmt(const ast::if_else_statement& in_stmt) -> middle::if_else_statement
	{
		middle::if_else_statement stmt;
		if (in_stmt.if_expr)
		{
			stmt.if_expr = wrap(convert_expr(*in_stmt.if_expr));
			stmt.if_expr->type = ctx.find_type("bool");
		}
		if (in_stmt.if_body)
			stmt.if_body = wrap(convert_stmt(*in_stmt.if_body));
		if (in_stmt.else_body)
			stmt.else_body = wrap(convert_stmt(*in_stmt.else_body));
		return stmt;
	}

	auto convert_stmt(const ast::while_statement& in_stmt) -> middle::while_statement
	{
		middle::while_statement stmt;
		if (in_stmt.cond_expr) {
			stmt.cond_expr = wrap(convert_expr(*in_stmt.cond_expr));
			stmt.cond_expr->type = ctx.find_type("bool");
		}
		if (in_stmt.loop_body)
			stmt.loop_body = wrap(convert_stmt(*in_stmt.loop_body));
		return stmt;
	}

	auto convert_stmt(const ast::return_statement& in_stmt) -> middle::return_statement
	{
		middle::return_statement stmt;
		if (in_stmt.value)
			stmt.value = wrap(convert_expr(*in_stmt.value));
		return stmt;
	}

	auto convert_stmt(const ast::empty_statement& /*in_stmt*/) -> middle::empty_statement
	{
		return {};
	}

	auto convert_stmt(const ast::expression& in_expr) -> middle::expression
	{
		return convert_expr(in_expr);
	}

	/*
	 * Expressions
	 */
	auto convert_expr(const ast::expression& in_expr) -> middle::expression
	{
		auto expr_visitor = [this] (const auto& expr) -> middle::expression
		{
			return convert_expr(expr);
		};
		return std::visit(expr_visitor, in_expr);
	}

	auto convert_expr(const ast::paren_expression& in_expr) -> middle::expression
	{
		return convert_expr(*in_expr.inner);
	}

	auto convert_expr(const ast::unary_expression& in_expr) -> middle::expression
	{
		auto expr = middle::expression(middle::unary_expression {
			.op = convert_operator(in_expr.op),
			.lhs = wrap(convert_expr(*in_expr.lhs)),
		});

		if (in_expr.op == ast::unary_operator::logical_negation)
			expr.type = ctx.find_type("bool");

		return expr;
	}

	auto convert_expr(const ast::binary_expression& in_expr) -> middle::binary_expression
	{
		return {
			.op = convert_operator(in_expr.op),
			.lhs = wrap(convert_expr(*in_expr.lhs)),
			.rhs = wrap(convert_expr(*in_expr.rhs)),
		};
	}

	auto convert_expr(const ast::call_expression& in_expr) -> middle::call_expression
	{
		middle::call_expression call;
		call.func_name = in_expr.func;
		for (const auto& arg : in_expr.args)
		{
			call.args.push_back(convert_expr(arg));
		}
		return call;
	}

	auto convert_expr(const ast::if_expression& in_expr) -> middle::if_expression
	{
		middle::if_expression expr;
		if (in_expr.if_expr)
			expr.if_expr = wrap(convert_expr(*in_expr.if_expr));
		if (in_expr.if_body)
			expr.if_body = wrap(convert_expr(*in_expr.if_body));
		if (in_expr.else_body)
			expr.else_body = wrap(convert_expr(*in_expr.else_body));
		return expr;
	}

	auto convert_expr(const ast::value_expression& in_expr) -> middle::value_expression
	{
		return { .name = in_expr.name };
	}

	auto convert_expr(const ast::numeric_literal& in_expr) -> middle::numeric_literal
	{
		return {
			.value = in_expr.value,
			.base = in_expr.base,
		};
	}

	auto convert_expr(const ast::string_literal& in_expr) -> middle::string_literal
	{
		return { .value = in_expr.value };
	}
};

} // namespace

auto convert_to_middle(context& ctx, const ast::module& in_mod) -> middle::module
{
	middle::module mod;

	convert_to_middle_visitor visitor{ .ctx = ctx };
	for (const auto& in_decl : in_mod.decls)
	{
		mod.decls.push_back(wrap(visitor.convert_decl(in_decl)));
	}

	return mod;
}

} // namespace aw::script
