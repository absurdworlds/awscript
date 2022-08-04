/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <aw/script/parser/parser.h>

#include "errors.h"

namespace aw::script {

parser::parser(lexer& lexer, diagnostics_engine& diag)
	: lex(lexer), diag(diag)
{
	tok = lexer.current();
}

bool parser::match(token_kind expected)
{
	if (tok.kind != expected)
		return false;

	// consume token
	tok = lex.next();
	return true;
}

bool parser::match(string_view identifier)
{
	if (tok.kind != token_kind::identifier)
		return false;

	if (tok.data != identifier)
		return false;

	// consume token
	tok = lex.next();
	return true;
}

bool parser::match_id(string_view identifier)
{
	assert(tok.kind == token_kind::identifier);

	if (tok.data != identifier)
		return false;

	// consume token
	tok = lex.next();
	return true;
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
	using namespace std::string_view_literals;

	// awscript employs context-sensitive keywords
	if (match_id("var"sv))
	{
		return parse_variable_declaration(ast::access::variable);
	}
	else if (match_id("const"sv))
	{
		return parse_variable_declaration(ast::access::constant);
	}
	else if (tok.data == "func"sv)
	{
		return parse_function_declaration();
	}
	else if (tok.data == "class"sv)
	{
		return parse_class_declaration();
	}

	return error(diag, diagnostic_id::expected_declaration, tok);
}

std::unique_ptr<ast::declaration> parser::parse_variable_declaration(ast::access access)
{
	return nullptr;
}

std::unique_ptr<ast::declaration> parser::parse_function_declaration()
{
	return nullptr;
}

std::unique_ptr<ast::declaration> parser::parse_class_declaration()
{
	return nullptr;
}

} // namespace aw::script
