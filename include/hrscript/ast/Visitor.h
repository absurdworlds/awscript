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
class FunctionProto;
class Function;
class Variable;
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
	virtual ~Visitor() = default;

	virtual void visit(ast::FunctionProto& node) {};
	virtual void visit(ast::Function& node) {};
	virtual void visit(ast::Variable& node) {};
	virtual void visit(ast::StatementBlock& node) {};
	virtual void visit(ast::IfElseStatement& node) {};
	virtual void visit(ast::NumberExpr& node) {};
	virtual void visit(ast::StringExpr& node) {};
	virtual void visit(ast::IdentifierExpr& node) {};
	virtual void visit(ast::CallExpr& node) {};
	virtual void visit(ast::UnaryExpr& node) {};
	virtual void visit(ast::BinaryExpr& node) {};
};

} // namespace ast
} // namespace deadalus
#endif//_hrscript_AST_Visitor_
