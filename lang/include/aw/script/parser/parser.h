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
#include <optional>

#include <aw/script/lexer/lexer.h>

#include <aw/script/parser/operators.h>

#include <aw/script/diag/diagnostics_engine.h>

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/expression.h>
#include <aw/script/ast/decl/module.h>
#include <aw/script/ast/decl/variable.h>
#include <aw/script/ast/decl/function.h>

#include <aw/script/symtab/symbol_table.h>

namespace aw::script {
namespace ast {
class type;
class expression;
} // namespace ast

// TODO: split it into declaration_parser, statement_parser and expression_parser

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
	bool advance();
	bool advance(token_kind expected);
	bool advance(string_view identifier);
	bool match(token_kind expected);
	bool match(string_view identifier);

	enum context {
		default_context,
		normal_module,
		foreign_module,
		if_condition,
	};
	context cur_context() const;
	void enter(context context);
	void leave(context context);

	/// Matches an identifier. Expects that the token kind was already checked.
	bool match_id(string_view identifier);

	std::string_view parse_identifier();
	std::string_view parse_type();

	enum decl_list_placement {
		top_level,
		braced_block,
	};

	auto parse_declaration_list(decl_list_placement placement) -> ast::declaration_list;
	std::unique_ptr<ast::declaration> parse_declaration();

	auto parse_module_declaration(ast::module_kind kind) -> std::unique_ptr<ast::declaration>;
	auto parse_module_body() -> ast::declaration_list;
	std::unique_ptr<ast::variable> parse_variable_declaration(ast::access access);
	std::unique_ptr<ast::declaration> parse_function_declaration();
	std::unique_ptr<ast::declaration> parse_class_declaration();

	std::unique_ptr<ast::function> parse_function_prototype();
	bool parse_function_arguments(ast::function& func);
	bool parse_function_return_type(ast::function& func);
	auto parse_function_body() -> std::unique_ptr<ast::statement>;

	auto parse_statement_block() -> std::unique_ptr<ast::statement>;
	std::unique_ptr<ast::statement> parse_statement();
	std::unique_ptr<ast::statement> parse_statement_inner();
	std::unique_ptr<ast::statement> parse_if_statement();
	std::unique_ptr<ast::statement> parse_for_statement();
	std::unique_ptr<ast::statement> parse_while_statement();
	auto parse_return_statement() -> std::unique_ptr<ast::statement>;

	std::unique_ptr<ast::expression> parse_expression();
	std::unique_ptr<ast::expression> parse_if_expression();
	std::unique_ptr<ast::expression> parse_unary_expression();
	std::unique_ptr<ast::expression> parse_binary_expression(
		std::unique_ptr<ast::expression> lhs, precedence min_prec);

	std::unique_ptr<ast::expression> parse_primary_expression();

	std::unique_ptr<ast::expression> parse_paren_expression();
	std::unique_ptr<ast::expression> parse_identifier_expression();
	std::unique_ptr<ast::expression> parse_string_literal_expression();
	std::unique_ptr<ast::expression> parse_numeric_literal_expression();
	auto parse_call_expression(std::string_view name) -> std::unique_ptr<ast::expression>;

	auto parse_unary_operator(token tok) -> std::optional<ast::unary_operator>;
	auto parse_binary_operator(token tok) -> std::optional<ast::binary_operator>;


private:
	/*! Lexer which provides the stream of tokens */
	lexer& lex;

	/*! Symbol table (for symbol lookups) */
	symbol_table& symtab;

	/*! Diagnostics engine for error reporting */
	diagnostics_engine& diag;

	/*! Current lookahead (peek) token. */
	token tok;

	/*! Context information */
	std::vector<context> context_stack;
};

} // namespace aw::script

#endif // aw_script_parser_h
