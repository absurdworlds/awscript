#ifndef generic_visitor_h
#define generic_visitor_h


#include "aw/script/semantic/semantic_analyzer.h"

#include "context.h"
#include "convert_to_middle.h"
#include "type_inference.h"

#include <aw/script/semantic/scope.h>
#include <aw/script/utility/wrap.h>

#include <aw/script/diag/error_t.h>

#include <aw/utility/ranges/paired.h>

namespace aw::script {
struct generic_visitor {
	/*
	 * Declarations
	 */
	void visit_decl(middle::declaration& decl)
	{
		auto ast_visitor = [this] (auto& decl)
		{
			return visit_decl(decl);
		};

		return std::visit(ast_visitor, decl);
	}

	void visit_decl(middle::function& func)
	{
		for (auto& var : func.args)
			visit_decl(*var);

		if (func.body)
			visit_stmt(*func.body);
	}

	void visit_decl(middle::variable& var)
	{
		if (var.value)
			visit_expr(*var.value);
	}

	void visit_decl(middle::struct_decl& decl)
	{
		for (auto& field : decl.members)
			visit_decl(*field);
	}

	/*
	 * Statements
	 */
	void visit_stmt(middle::statement& stmt)
	{
		auto stmt_visitor = [this] (auto& stmt)
		{
			return visit_stmt(stmt);
		};
		std::visit(stmt_visitor, stmt);
	}

	void visit_stmt(middle::decl_statement& stmt)
	{
		visit(*stmt.decl);
	}

	void visit_stmt(middle::statement_block& block)
	{
		for (auto& stmt : block)
			visit_stmt(stmt);
	}

	void visit_stmt(middle::if_else_statement& stmt)
	{
		if (stmt.if_expr)
			visit_stmt(*stmt.if_expr);
		if (stmt.if_body)
			visit_stmt(*stmt.if_body);
		if (stmt.else_body)
			visit_stmt(*stmt.else_body);
	}

	void visit_stmt(middle::while_statement& stmt)
	{
		if (stmt.cond_expr)
			visit_expr(*stmt.cond_expr);
		if (stmt.loop_body)
			visit_stmt(*stmt.loop_body);
	}

	void visit_stmt(middle::return_statement& stmt)
	{
		if (stmt.value)
			visit_expr(*stmt.value);

	}

	void visit_stmt(middle::empty_statement& /*stmt*/)
	{
	}

	void visit_stmt(middle::expression& expr)
	{
		visit_expr(expr);
	}

	/*
	 * Expressions
	 */
	void visit_expr(middle::expression& expr)
	{
		auto expr_visitor = [this, &ctx] (auto& expr)
		{
			return visit_expr(expr);
		};
		std::visit(expr_visitor, expr);
	}

	void visit_expr(middle::cast_expression& expr)
	{
		visit_expr(*expr.lhs);
	}

	void visit_expr(middle::unary_expression& expr)
	{
		visit_expr(*expr.lhs);
	}

	void visit_expr(middle::binary_expression& expr)
	{
		visit_expr(*expr.lhs);
		visit_expr(*expr.rhs);
	}

	void visit_expr(middle::call_expression& call)
	{
		for (auto& arg : call.args)
		{
			visit_expr(arg);
		}
	}

	void visit_expr(middle::field_expression& expr)
	{
		visit_expr(*expr.lhs);
	}

	void visit_expr(middle::if_expression& expr)
	{
		if (expr.if_expr)
			visit_expr(*expr.if_expr);
		if (expr.if_body)
			visit_expr(*expr.if_body);
		if (expr.else_body)
			visit_expr(*expr.else_body);
	}

	void visit_expr(middle::value_expression& expr)
	{
	}

	void visit_expr(middle::struct_literal& expr)
	{
		for (auto& field : expr.fields)
			visit_expr(*field.value);
	}

	};
} // namespace aw::script


#endif // generic_visitor_h
