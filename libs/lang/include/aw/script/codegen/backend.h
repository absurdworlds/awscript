/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_codegen_backend_h
#define aw_script_codegen_backend_h

#include <aw/types/string_view.h>

namespace aw::script {

class diagnostics_engine;

namespace ast {
class declaration;
}

// All of this is going to be changed in the future anyway, so
// I didn't put much thought into it. Feel free to modify
class backend {
public:
	virtual ~backend() = default;

	virtual bool set_target(string_view target_triple = {}) = 0;

	virtual bool handle_declaration(const ast::declaration& decl) = 0;

	virtual bool write_object_file(string_view out_path) = 0;
};


} // namespace aw::script

#endif//aw_script_codegen_backend_h
