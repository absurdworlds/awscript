/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_middle_statement_h
#define aw_script_ast_middle_statement_h

#include <memory>
#include <variant>
#include <vector>

#include <aw/script/ast/middle/expression.h>
#include <aw/script/utility/hard_alias.h>

namespace aw::script::middle {

struct statement;

using statement_list = std::vector<statement>;

struct empty_statement {
};

struct return_statement {
	std::unique_ptr<expression> value;
};

struct if_else_statement {
	std::unique_ptr<expression> if_expr;
	std::unique_ptr<statement>  if_body;
	std::unique_ptr<statement>  else_body;
};

using statement_block = hard_alias<statement_list>;

using statement_variant = std::variant<
	statement_block,
	if_else_statement,
	return_statement,
	expression,
	empty_statement
>;

struct statement : statement_variant
{
	using statement_variant::statement_variant;
	using statement_variant::operator=;
};

} // namespace aw::script::middle

#endif//aw_script_ast_middle_statement_h
