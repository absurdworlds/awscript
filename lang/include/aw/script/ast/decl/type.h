/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_type_h
#define aw_script_ast_decl_type_h

#include <aw/script/ast/declaration.h>

namespace aw::script::ast {

class type : public declaration {
public:
	type(string_view name)
	    : declaration(decl_kind::type, name)
	{}
};

} // namespace aw::script::ast

#endif // aw_script_ast_decl_type_h
