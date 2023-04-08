/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <vector>

#include <aw/script/ast/decl/variable.h>
#include <aw/script/ast/decl/function.h>
#include <aw/script/ast/decl/class.h>

#include <aw/script/ast/expression.h>
#include <aw/script/ast/expr/unary.h>
#include <aw/script/ast/expr/binary.h>
#include <aw/script/ast/expr/call.h>
#include <aw/script/ast/expr/number.h>
#include <aw/script/ast/expr/string.h>
#include <aw/script/ast/expr/identifier.h>

#include <aw/script/ast/statement.h>
#include <aw/script/ast/statement_block.h>
#include <aw/script/ast/if_else_statement.h>

#include <aw/script/utility/print_token.h>
#include <aw/script/parser/parser.h>

#include "errors.h"

namespace aw {
namespace script {
/*
 *      classDecl ::= 'class' id
 *          class ::= classDecl '{' classBodyDecls '}'
 * classBodyDecl  ::= functionDecl | varDecl
 * classBodyDecls ::= classBodyDecl ';' classBodyDecls?
 */
uptr<ast::Declaration>
Parser::parseClassDeclaration()
{
	// Class name
	if (!is_identifier(getNextToken()))
		return error(diag, Diagnostic::expected_identifier, token);

	std::string_view name = token.data();

	getNextToken(); // consume name;

	if (!match(token_kind::l_brace))
	{
		if (!match(token_kind::semicolon))
			return error_unexpected_token(diag, token, token_kind::semicolon);
		return std::make_unique<ast::ClassDeclaration>(name);
	}

	// Class members
	std::vector<uptr<ast::Variable>> members;
	while (match(kw_var)) {
		auto var = parse_variable();
		if (!var)
			return nullptr;

		members.emplace_back(std::move(var));

		if (!match(token_kind::semicolon))
			return error_unexpected_token(diag, token, token_kind::semicolon);
	}

	if (!match(token_kind::r_brace))
		return error_unexpected_token(diag, token, token_kind::r_brace);

	return std::make_unique<ast::ClassDeclaration>(name, std::move(members));
}

} // namespace script
} // namespace aw
