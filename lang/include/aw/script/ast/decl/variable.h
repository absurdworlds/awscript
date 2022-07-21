/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_variable_h
#define aw_script_ast_decl_variable_h

#include <aw/script/ast/decl/type.h>

namespace aw::script::ast {

enum class access {
	variable,
	constant
};

class variable : public declaration {
public:
	variable(string_view name, access access)
		: declaration(decl_kind::variable, name)
		, _access(access)
	{
	}

private:
	type* type;
	access _access;
};

} // namespace aw::script::ast

#endif // aw_script_ast_decl_variable_h
