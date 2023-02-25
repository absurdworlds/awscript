/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_module_h
#define aw_script_ast_decl_module_h

#include <aw/script/ast/declaration.h>

namespace aw::script::ast {

enum class module_kind {
	normal,
	foreign,
};

class module : public declaration {
public:
	explicit module(string_view name)
	    : declaration(decl_kind::type, name)
	{}

	module_kind kind = module_kind::normal;
	module* parent = nullptr;
	std::string_view language; // only for foreign modules
	declaration_list decls;
};

} // namespace aw::script::ast

#endif // aw_script_ast_decl_module_h
