/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <aw/script/parser/parser.h>

#include <aw/algorithm/in.h>
#include <aw/types/support/enum.h>

#include <aw/script/ast/decl/type.h>
#include <aw/script/ast/expression.h>
#include <aw/script/symtab/scope.h>

#include "errors.h"

using namespace std::string_view_literals;

namespace aw::script {

// TODO: evaluate idea
// sub-parsers, for each language construct
// with parse and bail-out methods
// bail-out: discard invalid source until you can parse again
// for example, func my_func( garbage_hsfjkflnkjashnlfkjl ) { }
// would discard `garbage_hsfjkflnkjashnlfkjl` and try to parse everything else

parser::parser(dependencies deps)
	: lex(deps.lexer), symtab(deps.symtab), diag(deps.diag)
{
	tok = lex.current();
}

bool parser::advance()
{
	tok = lex.next();
	return true; // TODO:
}

bool parser::advance(token_kind expected)
{
	assert(tok == expected);
	return advance();
}

bool parser::advance(string_view identifier)
{
	assert(tok == token_kind::identifier &&
	       tok == identifier);
	return advance();
}

bool parser::match(token_kind expected)
{
	if (tok.kind != expected)
		return false;

	// consume token
	advance();
	return true;
}

bool parser::match(string_view identifier)
{
	if (tok.kind != token_kind::identifier)
		return false;

	if (tok.data != identifier)
		return false;

	// consume token
	advance();
	return true;
}

bool parser::match_id(string_view identifier)
{
	assert(tok.kind == token_kind::identifier);

	if (tok.data != identifier)
		return false;

	// consume token
	advance();
	return true;
}

std::string_view parser::parse_identifier()
{
	// TODO: get rid of , ""
	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_type_name, tok);

	auto name = tok.data;
	advance();

	return name;
}

std::string_view parser::parse_type()
{
	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_type_name, tok);

#if 0 // we have to do this in 2 passes
	std::string_view parse_identifier();
	std::string_view parse_type(); anyway, so no point in making in more complicated
	auto symbol = symtab.lookup(tok.data);
	if (symbol)
	{
		if (!in(symbol->kind(), ast::decl_kind::type, ast::decl_kind::class_type))
		{
			return error(diag, diagnostic_id::expected_a_type, tok);
		}
		return static_cast<ast::type*>(symbol);
	}
#endif
	// TODO
	auto name = tok.data;
	tok = lex.next();

	return name;
}

std::unique_ptr<ast::declaration> parser::parse_top_level()
{
	if (tok == token_kind::eof)
		return nullptr;

	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_declaration, tok);

	std::unique_ptr<ast::declaration> decl = parse_declaration();

	/* TODO: do not forget about global variables*/
	while (match(token_kind::semicolon));

	return decl;
}

std::unique_ptr<ast::declaration> parser::parse_declaration()
{
	// awscript employs context-sensitive keywords
	if (match_id("var"sv))
		return parse_variable_declaration(ast::access::variable);

	if (match_id("const"sv))
		return parse_variable_declaration(ast::access::constant);

	if (match_id("func"sv))
		return parse_function_declaration();

	if (match_id("class"sv))
		return parse_class_declaration();

	return error(diag, diagnostic_id::expected_declaration, tok);
}

std::unique_ptr<ast::variable> parser::parse_variable_declaration(ast::access access)
{
	const auto type = parse_type();
	if (type.empty())
		return nullptr;

	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_identifier, tok);

	const auto name = tok.data;
	tok = lex.next();

	auto var = std::make_unique<ast::variable>(name, access);

	symtab.add_unresolved(type, &var->type);

	return var;
}

std::unique_ptr<ast::declaration> parser::parse_function_declaration()
{
	auto func = parse_function_prototype();

	if (tok == token_kind::l_brace) {
		// TODO: add error code
		func->body = parse_function_body();
	}

	return func;
}

std::unique_ptr<ast::declaration> parser::parse_class_declaration()
{
	return nullptr;
}

/*
 * functionDecl ::= 'func' id '(' args ')' (':' id)?
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
std::unique_ptr<ast::function> parser::parse_function_prototype()
{
	const auto name = parse_identifier();
	if (name.empty())
		return nullptr;

	auto func = std::make_unique<ast::function>(name);

	if (!match(token_kind::l_paren))
		return error_unexpected_token(diag, tok, token_kind::l_paren);

	func->scope = symtab.create_scope();

	parse_function_arguments(*func);

	if (!match(token_kind::r_paren))
		return error_unexpected_token(diag, tok, token_kind::r_paren);

	if (!parse_function_return_type(*func))
		return nullptr;

	return func;
}

bool parser::parse_function_arguments(ast::function& func)
{
	// TODO: support both 'var int a' and 'int a'
	// TODO: support const
	while (match("var"sv)) {
		auto arg = parse_variable_declaration(ast::access::variable);
		if (!arg)
			return {};

		func.add_arg(std::move(arg));

		if (tok == token_kind::r_paren)
			break;

		if (!match(token_kind::comma))
			// TODO: remove comma operator
			return error_unexpected_token(diag, tok, token_kind::comma);
	}

	return true;
}

bool parser::parse_function_return_type(ast::function& func)
{
	std::string_view type_name;

	if (match(token_kind::colon)) {
		type_name = parse_type();
		if (type_name.empty())
			return false;
	} else {
		type_name = "void";
	}

	symtab.add_unresolved(type_name, &func.return_type);
	return true;
}

/*
 * functionDef  ::= functionDecl '{' stmts '}'
 */
auto parser::parse_function_body() -> std::unique_ptr<ast::statement>
{
	return parse_statement_block();
}

/********************** Statements **********************/
auto parser::parse_statement_block() -> std::unique_ptr<ast::statement>
{
	if (!match(token_kind::l_brace))
		return error_unexpected_token(diag, tok, token_kind::l_brace);

	ast::statement_list statements;
	while (!match(token_kind::r_brace)) {
		auto statement = parse_statement();
		if (!statement)
			return {};

		statements.push_back(std::move(statement));
	}

	return std::make_unique<ast::statement>(std::move(statements));
}

std::unique_ptr<ast::statement> parser::parse_statement()
{
	auto stmt = parse_statement_inner();

	while (match(token_kind::semicolon));

	return stmt;
}

std::unique_ptr<ast::statement> parser::parse_statement_inner()
{
	switch (tok.kind)
	{
	case token_kind::semicolon:
		return std::make_unique<ast::statement>(ast::empty_statement());

	case token_kind::l_brace:
		return parse_statement_block();

	case token_kind::identifier:
		if (tok == "if"sv)
			return parse_if_statement();
		if (tok == "while"sv)
			return parse_while_statement();
		if (tok == "for"sv)
			return parse_while_statement();
		[[fallthrough]];
	default:
		auto expr = parse_expression();
		if (expr)
			return std::make_unique<ast::statement>(std::move(*expr));
		return nullptr;
	}
}

std::unique_ptr<ast::statement> parser::parse_if_statement()
{
	advance("if");

	ast::if_else_statement stmt;

	stmt.if_expr = parse_expression();
	if (!stmt.if_expr)
		return error_expected_expression(diag, tok);

	stmt.if_body = parse_statement();

	if (match("else"))
	{
		stmt.else_body = parse_statement();
	}

	return std::make_unique<ast::statement>(std::move(stmt));
}

std::unique_ptr<ast::statement> parser::parse_for_statement()
{
	return nullptr;
}

std::unique_ptr<ast::statement> parser::parse_while_statement()
{
	return nullptr;
}

/********************** Expressions **********************/
std::unique_ptr<ast::expression> parser::parse_expression()
{
	auto lhs = parse_unary_expression();
	if (!lhs)
		return nullptr;

	return parse_binary_expression(std::move(lhs), precedence::unknown);
}

std::unique_ptr<ast::expression> parser::parse_unary_expression()
{
	auto op = parse_unary_operator(tok);
	if (!op)
		return parse_primary_expression();

	ast::unary_expression expr {
		.op = *op,
		.lhs = parse_unary_expression()
	};

	if (!expr.lhs)
		return nullptr;

	return std::make_unique<ast::expression>(std::move(expr));
}

static precedence move_prec(precedence prec, int count)
{
	auto result = precedence( to_underlying(prec) + count );
	assert(result < precedence::max);
	return result;
}

auto parser::parse_binary_expression(std::unique_ptr<ast::expression> lhs, precedence min_prec) ->
	std::unique_ptr<ast::expression>
{
	while (true) {
		precedence cur_prec = token_precedence(tok);
		if (cur_prec < min_prec)
			break;

		auto op = parse_binary_operator(tok);
		if (!op)
			return nullptr;

		auto rhs = parse_unary_expression();
		if (!rhs)
			return nullptr;

		const precedence next_prec = token_precedence(tok);
		const bool is_right_assoc = is_right_associative(tok);

		if (cur_prec < next_prec ||
		   (cur_prec == next_prec && is_right_assoc)) {
			rhs = parse_binary_expression(
				std::move(rhs),
				move_prec(cur_prec, !is_right_assoc)
			);

			if (!rhs)
				return nullptr;
		}

		ast::binary_expression expr {
			.op = *op,
			.lhs = std::move(lhs),
			.rhs = std::move(rhs),
		};

		lhs = std::make_unique<ast::expression>(std::move(expr));

	}

	return lhs;
}

std::unique_ptr<ast::expression> parser::parse_primary_expression()
{
	switch(tok.kind) {
	case token_kind::l_paren:
		return parse_paren_expression();
	case token_kind::identifier:
		return parse_identifier_expression();
	case token_kind::numeric_constant:
		return parse_numeric_literal_expression();
	case token_kind::string_literal:
		return parse_string_literal_expression();
	default:
		return nullptr; // expected expression
	}
}

std::unique_ptr<ast::expression> parser::parse_paren_expression()
{
	advance(token_kind::l_paren);

	auto expr = parse_expression();

	if (!match(token_kind::r_paren))
		return error_unexpected_token(diag, tok, token_kind::r_paren); // expected ')'

	return expr;
}

std::unique_ptr<ast::expression> parser::parse_if_expression()
{
	advance("if");

	ast::if_expression expr;

	expr.if_expr = parse_expression();
	if (!expr.if_expr)
		return error_expected_expression(diag, tok);

	expr.if_body = parse_expression();

	if (!match("else"))
		return error_unexpected_token(diag, tok, "else"sv);

	expr.else_body = parse_expression();

	return std::make_unique<ast::expression>(std::move(expr));
}

std::unique_ptr<ast::expression> parser::parse_identifier_expression()
{
	if (tok == "if"sv)
		return parse_if_expression();

	std::string_view name = parse_identifier();
	if (name.empty())
		return nullptr;

	if (match(token_kind::l_paren))
		return parse_call_expression(name);

	// TODO: postfix operators ?

	ast::value_expression expr{ .name = name };

	return std::make_unique<ast::expression>(expr);
}

static string_view trim_string(string_view s)
{
	return s.substr(1, s.size() - 2);
}

std::unique_ptr<ast::expression> parser::parse_string_literal_expression()
{
	assert(tok == token_kind::string_literal &&
	       "parse_string_literal_expression called when there's no string literal!");

	ast::string_literal str{ .value = trim_string(tok.data) };

	// consume string
	advance();

	return std::make_unique<ast::expression>(str);
}

std::unique_ptr<ast::expression> parser::parse_numeric_literal_expression()
{
	assert(tok == token_kind::numeric_constant &&
	       "parse_numeric_literal_expression called when there's no number!");

	ast::numeric_literal num{ .value = tok.data };

	// consume number
	advance();

	return std::make_unique<ast::expression>(num);
}

auto parser::parse_call_expression(std::string_view name) -> std::unique_ptr<ast::expression>
{
	ast::call_expression expr;

	expr.func = name;

	if (!match(token_kind::r_paren)) {
		while (true) {
			auto arg = parse_expression();
			if (!arg)
				return nullptr;

			expr.args.push_back(std::move(*arg));

			if (match(token_kind::r_paren))
				break;

			if (!match(token_kind::comma))
				return error_unexpected_token(diag, tok, token_kind::comma); // expected ,
		}
	}

	return std::make_unique<ast::expression>(std::move(expr));
}

} // namespace aw::script
