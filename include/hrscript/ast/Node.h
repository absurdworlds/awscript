/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_Node_
#define _hrscript_AST_Node_
#include <hrscript/common/types.h>
#include <hrscript/ast/Visitor.h>
namespace hrscript {
namespace ast {
class Node {
public:
	virtual ~Node() = default;

	//! Implements Visitor Pattern
	virtual void accept(ast::Visitor& visitor) = 0;
};

} // namespace ast
} // namespace deadalus
#endif//_hrscript_AST_Node_
