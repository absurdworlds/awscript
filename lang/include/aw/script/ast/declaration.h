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

#include <aw/types/string_view.h>

#include <memory>
#include <vector>
#include <variant>

#include <cassert>

namespace aw::script::ast {

class scope;

struct type {
	std::string name;
};

enum class access {
	variable, // a.k.a mutable
	constant
};

struct variable {
	std::string name;
	std::string type;
	ast::access access;
};

struct statement;

using argument_list = std::vector<variable>;

struct function {
	// Ctors are only needed because of incomplete types
	function(std::string_view name);
	function(function&&);

	~function();

	function& operator=(function&&);

	std::string name;

	std::string return_type;
	argument_list args;

	std::unique_ptr<statement> body;
};

using declaration_variant = std::variant<
	std::monostate,
	variable,
	function,
	type
>;

class declaration : public declaration_variant {
public:
	declaration() = default;
	using declaration_variant::declaration_variant;
	using declaration_variant::operator=;
};

} // namespace aw::script::ast

#endif//aw_script_ast_decl_h
