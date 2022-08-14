/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_symtab_type_name_h
#define aw_script_symtab_type_name_h

#include <string_view>

#include <aw/script/utility/location.h>

namespace aw::script {

struct symbol_name {
	//! Place where the name is referenced
	location loc;

	std::string_view name;
};

} // namespace aw::script

#endif // aw_script_symtab_type_name_h
