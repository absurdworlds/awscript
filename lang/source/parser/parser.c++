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
	: lex(deps.lexer), diag(deps.diag)
{
	advance();
}

bool parser::advance()
{
	tok = lex.next();
	skip_comments();
	return true; // TODO:
}

void parser::skip_comments()
{
	while (in(tok, token_kind::line_comment, token_kind::block_comment))
		tok = lex.next();
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

struct parser::save_point {
	lexer::save_point lex_state;
	token tok;
};

auto parser::save_state() -> save_point
{
	return { lex.save_state(), tok };
}

void parser::restore_state(save_point sp)
{
	lex.restore_state(sp.lex_state);
	tok = sp.tok;
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
	advance();

	return name;
}

std::optional<ast::declaration> parser::parse_top_level()
{
	if (tok == token_kind::eof)
		return {};

	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_declaration, tok);

	std::optional<ast::declaration> decl = parse_declaration();

	/* TODO: do not forget about global variables*/
	while (match(token_kind::semicolon));

	return decl;
}

std::optional<ast::declaration> parser::parse_declaration()
{
	// awscript employs context-sensitive keywords
	if (match_id("var"sv))
		return parse_variable_declaration(ast::access::variable);

	if (match_id("const"sv))
		return parse_variable_declaration(ast::access::constant);

	if (match_id("function"sv))
		return parse_function_declaration();

	if (match_id("class"sv))
		return parse_class_declaration();

	return error(diag, diagnostic_id::expected_declaration, tok);
}


auto parser::parse_variable_declaration(ast::access access) -> std::optional<ast::variable>
{
	ast::variable var{ .access = access };
	var.name = parse_identifier();
	if (var.name.empty())
		return {};

	if (match(token_kind::colon)) {
		var.type = parse_type();
		if (var.type.empty())
			return {};
	}

	if (match(token_kind::equal)) {
		var.value = parse_expression();
		if (!var.value)
			return {};
	}

	return var;
}

auto parser::parse_function_declaration() -> std::optional<ast::function>
{
	auto func = parse_function_prototype();
	if (func) {
		func->body = parse_function_body();
	}

	return func;
}

auto parser::parse_class_declaration() -> std::optional<ast::declaration>
{
	return {};
}

/*
 * functionDecl ::= 'func' id '(' args ')' (':' id)?
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
auto parser::parse_function_prototype() -> std::optional<ast::function>
{
	const auto name = parse_identifier();
	if (name.empty())
		return {};

	ast::function func;
	func.name = name;

	if (!match(token_kind::l_paren))
		return error_unexpected_token(diag, tok, token_kind::l_paren);

	parse_function_arguments(func);

	if (!match(token_kind::r_paren))
		return error_unexpected_token(diag, tok, token_kind::r_paren);

	if (!parse_function_return_type(func))
		return {};

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

		func.args.push_back(std::move(*arg));

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

	func.return_type = type_name;
	return true;
}

/*
 * functionDef  ::= functionDecl '{' stmts '}'
 */
auto parser::parse_function_body() -> std::unique_ptr<ast::statement>
{
	if (match(token_kind::semicolon))
		return nullptr;
	if (tok == token_kind::l_brace)
		return parse_statement_block();
	if (tok == "return")
		return parse_return_statement();

#if AW_SCRIPT_FEATURE_IMPLICIT_RETURN
	if (is_declaration_start(tok)) // TODO
		return nullptr;

	ast::return_statement stmt;
	stmt.value = parse_expression();
	if (stmt.value)
		return std::make_unique<ast::statement>(std::move(stmt));

#endif
	return nullptr;
}

/********************** Statements **********************/
auto parser::parse_statement_block() -> std::unique_ptr<ast::statement>
{
	if (!match(token_kind::l_brace))
		return error_unexpected_token(diag, tok, token_kind::l_brace);

	ast::statement_block statements;
	while (!match(token_kind::r_brace)) {
		auto statement = parse_statement();
		if (!statement)
			return {};

		statements.push_back(std::move(*statement));
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
	switch (tok.kind) {
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
		if (tok == "return"sv)
			return parse_return_statement();
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

	match("then");

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

std::unique_ptr<ast::statement> parser::parse_return_statement()
{
	advance("return");

	ast::return_statement stmt;
	if (!match(token_kind::semicolon))
	{
		stmt.value = parse_expression();
		if (!stmt.value)
			return nullptr;
	}

	return std::make_unique<ast::statement>(std::move(stmt));
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

	match("then");

	expr.if_body = parse_expression();

	if (!match("else"))
		return error_unexpected_token(diag, tok, "else"sv);

	expr.else_body = parse_expression();

	return std::make_unique<ast::expression>(std::move(expr));
}

std::unique_ptr<ast::expression> parser::parse_identifier_expression()
{
	const auto sp = save_state();

	std::string_view name = parse_identifier();
	if (name.empty())
		return nullptr;

	if (name == "if"sv) {
		if (tok != token_kind::r_paren)
		{
			// Restoring state isn't necessary (and even slightly detrimental
			// for performance), but I think it's more elegant
			restore_state(sp);
			return parse_if_expression();
		}
	}

	if (match(token_kind::l_paren))
		return parse_call_expression(name);

	// TODO: postfix operators ?

	ast::value_expression expr{ .name = name };

	return std::make_unique<ast::expression>(expr);
}

char escape_char(char c)
{
	switch(c) {
	case 'n':
		return '\n';
	case 't':
		return '\t';
	case 'v':
		return '\v';
	case 'r':
		return '\r';
	case '\\':
	case '"':
	default:
		return c;
	}
}

static std::string parse_string(string_view s)
{
	std::string result;
	result.reserve(s.size());

	const auto str = s.substr(1, s.size() - 2);

	bool slash = false;
	for (char c : str)
	{
		if (c == '\\') {
			slash = true;
			continue;
		}

		if (slash) {
			slash = false;
			if (c == '\n')
				continue;
			c = escape_char(c);
		}

		result += c;
	}

	return result;
}

std::unique_ptr<ast::expression> parser::parse_string_literal_expression()
{
	assert(tok == token_kind::string_literal &&
	       "parse_string_literal_expression called when there's no string literal!");

	ast::string_literal str{ .value = parse_string(tok.data) };

	// consume string
	advance();

	return std::make_unique<ast::expression>(str);
}

auto decode_base(std::string_view num) -> ast::number_base
{
	using enum ast::number_base;
	if (num.starts_with('0')) {
		if (num.starts_with("0x"))
			return hexadecimal;
		else if (num.starts_with("0b"))
			return binary;
		else if (num.starts_with("0o"))
			return octal;
	}
	return decimal;
}

std::unique_ptr<ast::expression> parser::parse_numeric_literal_expression()
{
	assert(tok == token_kind::numeric_constant &&
	       "parse_numeric_literal_expression called when there's no number!");

	ast::numeric_literal num {
		.value = tok.data,
		.base = decode_base(tok.data)
	};

	if (num.base != ast::number_base::decimal)
		num.value.remove_prefix(2);

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
