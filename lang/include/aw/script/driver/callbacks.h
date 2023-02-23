/*
 * Copyright (C) 2023  Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_driver_callbacks_h
#define aw_script_driver_callbacks_h


namespace aw::script {
namespace ast {
class declaration;
} // namespace ast
namespace driver {
struct callbacks {
	virtual ~callbacks() = default;

	//! Called after a declaration was parsed
	virtual void on_parse_declaration(const ast::declaration& decl) {}

	//! Called for each declaration after everything was parsed
	virtual void process_declaration(const ast::declaration& decl) {}
};
} // namespace driver
} // namespace aw::script

#endif // aw_script_driver_callbacks_h
