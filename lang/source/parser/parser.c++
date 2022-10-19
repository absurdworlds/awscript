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

std::string_view parser::parse_identifier()
{
	// TODO: get rid of , ""
	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_type_name, tok), "";


	std::string_view parse_identifier();
	std::string_view parse_type();
	auto name = tok.data;
	tok = lex.next();

	return name;
}

std::string_view parser::parse_type()
{
	if (tok != token_kind::identifier)
		return error(diag, diagnostic_id::expected_type_name, tok), "";

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

	func->args = parse_function_arguments();

	if (!match(token_kind::r_paren))
		return error_unexpected_token(diag, tok, token_kind::r_paren);

	if (!parse_function_return_type(*func))
		return nullptr;

	return func;
}

ast::argument_list parser::parse_function_arguments()
{
	using namespace std::string_view_literals;
	// TODO: return optional<> on error
	// TODO: return argument_list, status for partial error recovery

	ast::argument_list args;

	// TODO: support both 'var int a' and 'int a'
	// TODO: support const
	while (match("var"sv)) {
		auto arg = parse_variable_declaration(ast::access::variable);
		if (!arg)
			return {};

		args.push_back(std::move(arg));

		if (tok == token_kind::r_paren)
			break;

		if (!match(token_kind::comma))
			// TODO: remove comma operator
			return error_unexpected_token(diag, tok, token_kind::comma), std::move(args);
	}

	return args;
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
std::unique_ptr<ast::statement_block> parser::parse_function_body()
{
	return parse_statement_block();
}

std::unique_ptr<ast::statement_block> parser::parse_statement_block()
{
	if (!match(token_kind::l_brace))
		return error_unexpected_token(diag, tok, token_kind::l_brace);

	ast::statement_list statements;
	while (!match(token_kind::r_brace)) {
		auto statement = parse_statement();
		if (!statement)
			return nullptr;

		statements.push_back(std::move(statement));
	}

	return std::make_unique<ast::statement_block>(std::move(statements));
}

std::unique_ptr<ast::statement> parser::parse_statement()
{
	return nullptr;
}


} // namespace aw::script
