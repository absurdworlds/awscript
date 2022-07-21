/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_function_h
#define aw_script_ast_decl_function_h

#include <vector>
#include <memory>

#include <aw/script/ast/decl/type.h>
#include <aw/script/ast/decl/variable.h>

namespace aw::script::ast {

using argument_list = std::vector<std::unique_ptr<variable>>;

class function : public declaration
{
public:
	function()
	    : declaration(decl_kind::function)
	{}

private:

	argument_list args;
};

} // namespace aw::script::ast

#endif // aw_script_ast_decl_function_h
