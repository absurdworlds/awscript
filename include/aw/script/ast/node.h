/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 *
 */
#ifndef aw_script_ast_node_h
#define aw_script_ast_node_h
#include <aw/script/common/types.h>
#include <aw/script/ast/visitor.h>
namespace aw {
namespace script {
namespace ast {
class Node {
public:
	virtual ~Node() = default;

	//! Implements Visitor Pattern
	virtual void accept(ast::Visitor& visitor) = 0;
};

} // namespace ast
} // namespace script
} // namespace aw
#endif//aw_script_ast_node_h
