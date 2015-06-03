/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_StatementBlock_
#define _daedalus_AST_StatementBlock_
#include <daedalus/ast/Statement.h>

namespace daedalus {
namespace ast {
/* TODO: Does Daedalus allow “freestanding” blocks?
 * {
 *    ...
 * }
 *
 * My memory tells me it doesn't. If so, I probably should
 * change base class
 */
class StatementBlock : public Statement {
public:
	StatementBlock();
	virtual ~StatementBlock();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::vector<Statement*> statements;
};
} // namespace ast
} // namespace daedalus
#endif//_daedalus_AST_StatementBlock_
