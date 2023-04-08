#include "ast_printer_default.h"

// TODO: for now it prints into iostream
// abstract it out later
#include <iostream>

namespace aw::script {
ast_printer_default::ast_printer_default()
{

}

void ast_printer_default::print_declaration(const ast::declaration& decl)
{
	std::visit([this] (auto&& node) { print_decl(node); }, decl);
}

void ast_printer_default::start(string_view name, scope_type type)
{
	print_inline("(");
	if (name.empty())
		state = node_start;
	else
		std::cout << name;
	if (type == block_scope)
		start_line();
	scope_stack.push_back(type);
}

void ast_printer_default::end()
{
	const auto type = scope_stack.back();
	scope_stack.pop_back();
	print_indent();
	std::cout << ')';
	if (type != inline_scope)
		start_line();
}

void ast_printer_default::start_line()
{
	state = line_start;
	std::cout << '\n';
}


void ast_printer_default::print_inline(string_view text)
{
	if (state == line_start)
		print_indent();
	if (state == line_middle)
		std::cout << ' ';
	state = line_middle;
	std::cout << text;
}

void ast_printer_default::print_indent()
{
	if (state != line_start)
		return;
	const auto depth = scope_stack.size();
	if (depth > 0)
		std::cout << std::string(depth*2, ' ');
}

void ast_printer_default::print_decl(const ast::function& decl)
{
	start("func", mixed_scope);

	print_type(decl.return_type);
	print_inline(decl.name);
	start_line();
	{
		start("fn-args");
		for (const auto& param : decl.params)
		{
			print_decl(param);
			start_line();
		}
		end();
	}
	{
		start("fn-body");
		if (decl.body)
			print(*decl.body);
		end();
	}
	end();
}

void ast_printer_default::print_decl(const ast::variable& var)
{
	start("var", inline_scope);
	print_type(var.type);
	print_inline(var.name);
	end();
}

void ast_printer_default::print(const ast::statement& stmt)
{
	std::visit([this] (auto&& node) { print_stmt(node); }, stmt);
}

void ast_printer_default::print_type(const ast::type& type)
{
	if (auto _ = get_if<ast::unknown_type>(&type)) {
		print_inline("<inferred type>");
	} else if (auto ty = get_if<ast::pointer_type>(&type)) {
		print_inline(ty->pointee + "*");
	} else if (auto ty = get_if<ast::reference_type>(&type)) {
		print_inline(ty->pointee + "&");
	} else {
		assert(!"Forgot to update the ast printer?");
		print_inline("<unresolved type>");
	}
}

void ast_printer_default::print_stmt(const ast::decl_statement& stmt)
{
	if (stmt.decl)
		print_declaration(*stmt.decl);
}

void ast_printer_default::print_stmt(const ast::return_statement& stmt)
{
	start("return", inline_scope);
	print_expr(stmt.value);
	end();
}

void ast_printer_default::print_stmt(const ast::statement_list& list)
{
	start("stmt-list");
	for (const auto& stmt : list)
	{
		print(stmt);
	}
	end();
}

void ast_printer_default::print_stmt(const ast::if_else_statement& stmt)
{
	start("if", mixed_scope);

	print_expr(stmt.if_expr);
	start_line();

	{
		start("then", block_scope);
		if (stmt.if_body)
			print(*stmt.if_body);
		end();
	}

	if (stmt.else_body)
	{
		start("else", block_scope);
		print(*stmt.else_body);
		end();
	}

	end();
}

void ast_printer_default::print_stmt(const ast::while_statement& stmt)
{
	start("while", mixed_scope);

	print_expr(stmt.cond_expr);
	start_line();
	{
		start("do", block_scope);
		if (stmt.loop_body)
			print(*stmt.loop_body);
		end();
	}
	end();
}

void ast_printer_default::print_stmt(const ast::empty_statement& stmt)
{
	start("", inline_scope);
	end();
}

void ast_printer_default::print_stmt(const ast::expression& expr)
{
	print_expr(expr);
	start_line();
}

void ast_printer_default::print_expr(const std::unique_ptr<ast::expression>& expr)
{
	if (expr)
		print_expr(*expr);
	else
		print_inline("<invalid-expr>");
}

void ast_printer_default::print_expr(const ast::expression& expr)
{
	std::visit([this] (auto&& node) { print_expr(node); }, expr);
}

void ast_printer_default::print_expr(const ast::paren_expression& expr)
{
	start("group", inline_scope);

	print_expr(expr.inner);

	end();
}

void ast_printer_default::print_expr(const ast::if_expression& expr)
{
	start("if", inline_scope);

	print_expr(expr.if_expr);
	start_line();

	{
		start("then", inline_scope);
		if (expr.if_body)
			print_expr(*expr.if_body);
		end();
	}

	{
		start("else", inline_scope);
		if (expr.else_body)
			print_expr(*expr.else_body);
		end();
	}

	end();
}

std::string_view spell_operator(ast::unary_operator op)
{
	switch(op) {
	case ast::unary_operator::minus:
		return "-";
	case ast::unary_operator::plus:
		return "+";
	case ast::unary_operator::logical_negation:
		return "!";
	case ast::unary_operator::binary_negation:
		return "~";
	}
	return "<invalid-unary-op>";
}

void ast_printer_default::print_expr(const ast::unary_expression& expr)
{
	start(spell_operator(expr.op), inline_scope);
	print_expr(expr.lhs);
	end();
}

std::string_view spell_operator(ast::binary_operator op)
{
	switch(op) {
	case ast::binary_operator::minus:
		return "-";
	case ast::binary_operator::plus:
		return "+";
	case ast::binary_operator::multiply:
		return "*";
	case ast::binary_operator::divide:
		return "/";
	case ast::binary_operator::less:
		return "<";
	case ast::binary_operator::greater:
		return ">";
	case ast::binary_operator::assignment:
		return "=";
	}
	return "<invalid-binary-op>";
}

void ast_printer_default::print_expr(const ast::binary_expression& expr)
{
	start(spell_operator(expr.op), inline_scope);
	print_expr(expr.lhs);
	print_expr(expr.rhs);
	end();
}

void ast_printer_default::print_expr(const ast::call_expression& expr)
{
	start("call", inline_scope);
	print_inline(expr.func);
	{
		start({}, inline_scope);
		for (const auto& arg : expr.args)
			print_expr(arg);
		end();
	}
	end();
}

void ast_printer_default::print_expr(const ast::value_expression& expr)
{
	print_inline(expr.name);
}

void ast_printer_default::print_expr(const ast::numeric_literal& expr)
{
	print_inline(expr.value);
}

void ast_printer_default::print_expr(const ast::string_literal& expr)
{
	// TODO: escape special chars
	std::string tmp;
	tmp += '"';
	tmp += expr.value;
	tmp += '"';
	print_inline(tmp);
}

} // namespace aw::script
