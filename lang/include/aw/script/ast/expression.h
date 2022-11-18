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

#include <aw/script/ast/statement.h>

#include <variant>

// TODO: expression inherits from a statement so that I don't have to make
// a separate expr_statement type, but I am not sure it this is the best way
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

class expression : public statement {
public:
	virtual ~expression() = default;

	template<typename T> // TODO: remove, turn expression into using
	expression(T&& tmp)
		: operand(std::move(tmp))
	{
	}

	std::variant<
		unary_expression,
		binary_expression,
		value_expression> operand;
};

} // namespace aw::script::ast

#endif//aw_script_ast_expression_h
