/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_expression_h
#define aw_script_ast_expression_h

#include <aw/script/ast/statement.h>

// TODO: expression inherits from a statement so that I don't have to make
// a separate expr_statement type, but I am not sure it this is the best way
namespace aw::script::ast {

class expression : public statement {
public:
	virtual ~expression() = default;
};

} // namespace aw::script::ast

#endif//aw_script_ast_expression_h
