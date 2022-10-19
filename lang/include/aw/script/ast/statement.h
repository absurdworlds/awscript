/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_statement_h
#define aw_script_ast_statement_h

namespace aw::script::ast {

class statement {
public:
	virtual ~statement() = default;
};

} // namespace aw::script::ast

#endif//aw_script_ast_statement_h
