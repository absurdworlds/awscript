/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_if_else_statement_h
#define aw_script_ast_if_else_statement_h

#include <memory>
#include <aw/script/ast/expression.h>
#include <aw/script/ast/statement.h>

// TODO: optimize if-else if-else chain

namespace aw::script::ast {

class if_else_statement : public statement {
public:

	std::unique_ptr<expression> if_expr;
	std::unique_ptr<statement>  if_body;
	std::unique_ptr<statement>  else_body;
};

} // namespace aw::script::ast

#endif//aw_script_ast_if_else_statement_h
