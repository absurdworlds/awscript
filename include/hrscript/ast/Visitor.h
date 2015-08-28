/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_Visitor_
#define _hrscript_AST_Visitor_
namespace hrscript {
namespace ast {
class Node;
class FuncDeclaration;
class FuncDefinition;
class VariableDeclaration;
class StatementBlock;
class IfElseStatement;
class NumberExpr;
class StringExpr;
class IdentifierExpr;
class CallExpr;
class UnaryExpr;
class BinaryExpr;

class Visitor {
public:
	virtual ~Visitor();

	virtual void visit(ast::FuncDeclaration& node) = 0;
	virtual void visit(ast::FuncDefinition& node) = 0;
	virtual void visit(ast::VariableDeclaration& node) = 0;
	virtual void visit(ast::StatementBlock& node) = 0;
	virtual void visit(ast::IfElseStatement& node) = 0;
	virtual void visit(ast::NumberExpr& node) = 0;
	virtual void visit(ast::StringExpr& node) = 0;
	virtual void visit(ast::IdentifierExpr& node) = 0;
	virtual void visit(ast::CallExpr& node) = 0;
	virtual void visit(ast::UnaryExpr& node) = 0;
	virtual void visit(ast::BinaryExpr& node) = 0;
};

} // namespace ast
} // namespace deadalus
#endif//_hrscript_AST_Visitor_
