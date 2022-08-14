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

#include <aw/script/ast/decl/type.h>

#include "errors.h"

namespace aw::script {

parser::parser(dependencies deps)
	: lex(deps.lexer), symtab(deps.symtab), diag(deps.diag)
{
	tok = lex.current();
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
		return parse_variable_declaration(ast::access::variable);

	if (match_id("const"sv))
		return parse_variable_declaration(ast::access::constant);

	if (match_id("func"sv))
		return parse_function_declaration();

	if (match_id("class"sv))
		return parse_class_declaration();

	return error(diag, diagnostic_id::expected_declaration, tok);
}

std::unique_ptr<ast::declaration> parser::parse_variable_declaration(ast::access access)
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
	return nullptr;
}

std::unique_ptr<ast::declaration> parser::parse_class_declaration()
{
	return nullptr;
}

std::string_view parser::parse_type()
{
	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_type_name, tok);

#if 0 // we have to do this in 2 passes anyway, so no point in making in more complicated
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

} // namespace aw::script
