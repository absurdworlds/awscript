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

class type;
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
	multiply,
	divide,
	less,
	greater,
	assignment,
};

struct binary_expression {
	binary_operator op;
	std::unique_ptr<expression> lhs;
	std::unique_ptr<expression> rhs;
};

// TODO: tell apart lvalue and rvalue
struct value_expression {
	std::string_view name;
};

// struct is needed due to clang's shenanigans
struct argument_list : std::vector<expression> {
	argument_list() = default;

	argument_list(const argument_list&) = delete;
	argument_list(argument_list&&) = default;

	argument_list& operator=(const argument_list&) = default;
	argument_list& operator=(argument_list&&) = default;
};

struct call_expression {
	std::string_view func;
	argument_list args;
};

struct numeric_literal {
	std::string_view value;
	int base = 10;
};

struct string_literal {
	std::string value;
};

struct if_expression {
	std::unique_ptr<expression> if_expr;
	std::unique_ptr<expression> if_body;
	std::unique_ptr<expression> else_body;
};

using expression_variant = std::variant<
	unary_expression,
	binary_expression,
	call_expression,
	if_expression,
	value_expression,
	numeric_literal,
	string_literal
>;

struct expression : expression_variant
{
	using expression_variant::expression_variant;
	using expression_variant::operator=;
};

} // namespace aw::script::ast

#endif//aw_script_ast_expression_h
