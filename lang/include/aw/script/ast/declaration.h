/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_h
#define aw_script_ast_decl_h

#include <aw/script/ast/mutability.h>
#include <aw/script/ast/statement.h>
#include <aw/script/ast/type.h>

#include <aw/types/string_view.h>

#include <memory>
#include <variant>
#include <vector>
#include <optional>

#include <cassert>

namespace aw::script::ast {

struct variable {
	std::string name;
	ast::type type;
	ast::access access;
	std::unique_ptr<expression> value;
};

struct variadic_parameter {
	std::string_view name;
};

struct parameter_list : std::vector<variable>
{
	std::optional<variadic_parameter> variadic;
};

struct function {
	std::string name;

	ast::type return_type;
	parameter_list params;

	std::unique_ptr<statement> body;

	bool is_variadic() const { return params.variadic.has_value(); }
};

using var_list = hard_alias<std::vector<variable>>;
struct struct_decl {
	std::string_view name;

	var_list members;
};

struct declaration;

struct module {
	std::vector<declaration> decls;
};

using declaration_variant = std::variant<
	variable,
	function,
	struct_decl
>;

struct declaration : public declaration_variant {
	using declaration_variant::declaration_variant;
	using declaration_variant::operator=;
};

} // namespace aw::script::ast

#endif//aw_script_ast_decl_h
