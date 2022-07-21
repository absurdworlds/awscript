/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_parser_h
#define aw_script_parser_h

#include <memory>

#include <aw/script/lexer/lexer.h>

#include <aw/script/diag/diagnostics_engine.h>

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/decl/variable.h>

namespace aw::script {

class parser {
public:
	parser(lexer& lexer, diagnostics_engine& diag);

	~parser() = default;

	std::unique_ptr<ast::declaration> parse_top_level();

private:
	bool match(token_kind expected);

	std::unique_ptr<ast::declaration> parse_declaration();

	std::unique_ptr<ast::declaration> parse_variable_declaration(ast::access access);
	std::unique_ptr<ast::declaration> parse_function_declaration();
	std::unique_ptr<ast::declaration> parse_class_declaration();

private:
	/*! Current lookahead (peek) token. */
	token tok;

	/*! Lexer which provides the stream of tokens */
	lexer& lex;

	/*! Diagnostics engine for error reporting */
	diagnostics_engine& diag;
};

} // namespace aw::script

#endif // aw_script_parser_h
