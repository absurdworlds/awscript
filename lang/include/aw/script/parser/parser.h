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
#include <aw/script/ast/statement.h>

#include <aw/script/diag/error_t.h>

namespace aw::script {

template<typename T>
struct result {
	T value;
	error_t error;

	explicit operator bool() const
	{
		return !error.has_value();
	}
};

template<typename T>
struct opt_result {
	template<typename U> requires std::is_constructible_v<error_t, U>
	opt_result(U e)
		: error(e)
	{}

	template<typename U> requires std::is_constructible_v<T, U>
	opt_result(U v, error_t e = {})
		: value(std::move(v))
		, error(e)
	{}

	opt_result(result<T> r)
		: opt_result(std::move(r.value), r.error)
	{}

	std::optional<T> value;
	error_t error;

	T& operator*() { return *value; }

	explicit operator bool() const { return value.has_value(); }
};

// TODO: split it into declaration_parser, statement_parser and expression_parser

class parser {
public:
	struct dependencies {
		script::lexer& lexer;
		diagnostics_engine& diag;
	};

	parser(dependencies deps);

	~parser() = default;

	auto parse_top_level() -> std::optional<ast::declaration>;

private:
	bool advance();
	bool advance(token_kind expected);
	bool advance(string_view identifier);

	struct save_point;

	auto save_state() -> save_point;

	void restore_state(save_point sp);

	void skip_comments();
	bool match(token_kind expected);
	bool match(string_view identifier);

	/// Matches an identifier. Expects that the token kind was already checked.
	bool match_id(string_view identifier);

	std::string_view parse_identifier();
	bool parse_type_specifier(ast::type& type, ast::type default_type);
	auto parse_type() -> std::optional<ast::type>;

	auto parse_declaration() -> std::optional<ast::declaration>;

	auto parse_variable_declaration(ast::access access) -> std::optional<ast::variable>;
	auto parse_struct_initializer() -> result<ast::expression>;
	auto parse_variable_initializer() -> opt_result<ast::expression>;

	auto parse_struct_declaration() -> std::optional<ast::declaration>;
	auto parse_class_declaration() -> std::optional<ast::declaration>;

	auto parse_function_declaration() -> std::optional<ast::function>;
	auto parse_function_prototype() -> std::optional<ast::function>;
	bool parse_variadic_parameter(ast::function& func);
	auto parse_variable_start() -> std::optional<ast::access>;
	bool parse_function_arguments(ast::function& func);
	bool parse_function_return_type(ast::function& func);
	auto parse_function_body() -> std::optional<ast::statement>;

	auto parse_statement() -> std::optional<ast::statement>;
	auto parse_statement_inner() -> std::optional<ast::statement>;
	auto parse_statement_block() -> std::optional<ast::statement>;
	auto parse_if_statement() -> std::optional<ast::statement>;
	auto parse_for_statement() -> std::optional<ast::statement>;
	auto parse_while_statement() -> std::optional<ast::statement>;
	auto parse_return_statement() -> std::optional<ast::statement>;
	auto parse_local_variable(ast::access access) -> std::optional<ast::decl_statement>;

	auto parse_expression() -> opt_result<ast::expression>;
	auto parse_if_expression() -> result<ast::expression>;
	auto parse_postfix_expression() -> opt_result<ast::expression>;
	auto parse_unary_expression() -> opt_result<ast::expression>;
	auto parse_as_expression(ast::expression lhs) -> result<ast::expression>;
	auto parse_binary_expression(ast::expression lhs, precedence min_prec)
		-> result<ast::expression>;

	auto parse_primary_expression() -> opt_result<ast::expression>;

	auto parse_paren_expression() -> result<ast::expression>;
	auto parse_identifier_expression() -> result<ast::expression>;
	auto parse_string_literal_expression() -> ast::string_literal;
	auto parse_numeric_literal_expression() -> ast::numeric_literal;
	auto parse_call_expression(std::string_view name) -> result<ast::expression>;

	auto parse_unary_operator(token tok) -> std::optional<ast::unary_operator>;
	auto parse_binary_operator(token tok) -> std::optional<ast::binary_operator>;

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
