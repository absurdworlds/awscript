/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_expression_h
#define aw_script_ast_expression_h

#include <memory>
#include <variant>
#include <vector>

namespace aw::script::ast {

class expression;

enum class unary_operator {
	minus,
	plus,
	logical_negation,
	binary_negation,
};

struct unary_expression {
	unary_operator op;
	std::unique_ptr<expression> lhs;
};

enum class binary_operator {
	minus,
	plus,
};

struct binary_expression {
	unary_operator op;
	std::unique_ptr<expression> lhs;
	std::unique_ptr<expression> rhs;
};

struct value_expression {
	std::string_view name;
};

struct call_expression {
	std::string_view func;
	std::vector<expression> args;
};

struct numeric_literal {
	std::string_view value;
};

struct string_literal {
	std::string_view value;
};

using expression_variant = std::variant<
	unary_expression,
	binary_expression,
	call_expression,
	value_expression,
	numeric_literal,
	string_literal
>;

struct expression : expression_variant
{
	template<typename T>
	explicit expression(T&& data) : expression_variant(std::forward<T>(data)) {}
};

} // namespace aw::script::ast

#endif//aw_script_ast_expression_h
