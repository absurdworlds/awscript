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
#include <aw/script/ast/decl/function.h>

#include <aw/script/symtab/symbol_table.h>

namespace aw::script {
namespace ast {
class type;
} // namespace ast

class parser {
public:
	struct dependencies {
		script::lexer& lexer;
		symbol_table& symtab;
		diagnostics_engine& diag;
	};

	parser(dependencies deps);

	~parser() = default;

	std::unique_ptr<ast::declaration> parse_top_level();

private:
	bool match(token_kind expected);
	bool match(string_view identifier);

	/// Matches an identifier. Expects that the token kind was already checked.
	bool match_id(string_view identifier);

	std::unique_ptr<ast::declaration> parse_declaration();

	std::unique_ptr<ast::variable> parse_variable_declaration(ast::access access);
	std::unique_ptr<ast::declaration> parse_function_declaration();
	std::unique_ptr<ast::declaration> parse_class_declaration();

	std::unique_ptr<ast::function> parse_function_prototype();
	ast::argument_list parse_function_arguments();

	std::string_view parse_identifier();
	std::string_view parse_type();

private:
	/*! Current lookahead (peek) token. */
	token tok;

	/*! Lexer which provides the stream of tokens */
	lexer& lex;

	/*! Symbol table (for symbol lookups) */
	symbol_table& symtab;

	/*! Diagnostics engine for error reporting */
	diagnostics_engine& diag;
};

} // namespace aw::script

#endif // aw_script_parser_h
