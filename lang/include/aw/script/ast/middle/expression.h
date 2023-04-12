/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_middle_expression_h
#define aw_script_ast_middle_expression_h

#include <aw/script/ast/number_base.h>
#include <aw/script/ir/type.h>

#include <aw/script/ir/operators.h>
#include <aw/script/utility/value_ptr.h>

#include <memory>
#include <variant>
#include <vector>

namespace aw::script::middle {

class expression;

struct cast_expression {
	ir::type* to_type;
	value_ptr<expression> lhs;
};

struct unary_expression {
	ir::unary_operator op;
	value_ptr<expression> lhs;
};

struct binary_expression {
	ir::binary_operator op;
	value_ptr<expression> lhs;
	value_ptr<expression> rhs;
};

// TODO: tell apart lvalue and rvalue
class declaration;
class variable;
struct value_expression {
	std::string_view name;
	variable* ref;
};

class function;
using argument_list = std::vector<expression>;
struct call_expression {
	function* func = nullptr;
	std::string_view func_name;
	argument_list args;
};

struct field_expression {
	ir::struct_type* type = nullptr;
	value_ptr<expression> lhs;
	std::string name;
};

struct numeric_literal {
	std::string_view value;
	ir::type* type = nullptr;
	ast::number_base base = ast::number_base::decimal;
};

struct bool_literal {
	bool value = false;
};

struct string_literal {
	std::string value;
};

struct struct_literal {
	ir::struct_type* type = nullptr;
	struct field {
		std::string name;
		value_ptr<expression> value;
	};
	using field_vec = std::vector<field>;
	field_vec fields;
};

struct if_expression {
	value_ptr<expression> if_expr;
	value_ptr<expression> if_body;
	value_ptr<expression> else_body;
};

using expression_variant = std::variant<
	cast_expression,
	unary_expression,
	binary_expression,
	call_expression,
	field_expression,
	if_expression,
	value_expression,
	numeric_literal,
	bool_literal,
	string_literal,
	struct_literal
>;

struct expression : expression_variant
{
	ir::type* type = nullptr;

	using expression_variant::expression_variant;
	using expression_variant::operator=;
};

} // namespace aw::script::middle

#endif//aw_script_ast_middle_expression_h
