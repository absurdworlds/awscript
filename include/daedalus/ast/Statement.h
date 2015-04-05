/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_Statement_
#define _daedalus_AST_Statement_
#include <daedalus/ast/Node.h>

namespace daedalus {
namespace ast {

class Statement : public Node {
public:
	virtual ~Statement();

	virtual accept(ast::Visitor& visitor);
};

} // namespace ast
} // namespace deadalus
#endif//_daedalus_AST_Statement_
