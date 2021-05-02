/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_AST_Conditional_
#define aw_script_AST_Conditional_
#include <aw/script/ast/statement.h>

namespace aw {
namespace script {
namespace ast {
class Exression;
/*!
 * ifStmt = 'if' '(' expr ')' block optElse
 * optElse = ('else' block) | ϵ
 */
class IfElseStatement : public Statement {
public:
	IfElseStatement(uptr<Expression> ifExpr,
	                uptr<Statement>  ifBody,
	                uptr<Statement> elseBody)
		: ifExpr(std::move(ifExpr)),
		  ifBody(std::move(ifBody)),
		  elseBody(std::move(elseBody))
	{
	}

	virtual ~IfElseStatement() = default;

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	Expression& getCondition()
	{
		return *ifExpr;
	}

	Statement& getThenBranch()
	{
		return *ifBody;
	}

	Statement* getElseBranch()
	{
		return elseBody.get();
	}
protected:
	uptr<Expression> ifExpr;
	uptr<Statement>  ifBody;
	uptr<Statement>  elseBody;
};

} // namespace ast
} // namespace script
} // namespace aw
#endif//aw_script_AST_Conditional_
