/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_number_base_h
#define aw_script_ast_number_base_h

namespace aw::script::ast {

enum class number_base {
	binary = 2,
	octal = 8,
	decimal = 10,
	hexadecimal = 16,
};

} // namespace aw::script::ast

#endif // aw_script_ast_number_base_h
