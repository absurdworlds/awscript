/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */

#include <hrscript/ast/decl/Variable.h>
#include <hrscript/ast/decl/Function.h>

#include <hrscript/ast/UnaryExpr.h>
#include <hrscript/ast/BinaryExpr.h>
#include <hrscript/ast/CallExpr.h>
#include <hrscript/ast/NumberExpr.h>
#include <hrscript/ast/StringExpr.h>
#include <hrscript/ast/IdentifierExpr.h>

#include <hrscript/ast/IfElseStatement.h>
#include <hrscript/ast/StatementBlock.h>

#include <hrscript/utility/Printer.h>
#include <hrscript/utility/PrintToken.h>

namespace hrscript {
Printer::Printer(awrts::io::WriteStream& out)
	: writer(awrts::hdf::createWriter(out))
{
}

void Printer::printSignature(ast::FuncDeclaration& node)
{
	writer->writeValue("name", node.getName());
	writer->writeValue("return", node.getReturnType());

	writer->startNode("arguments");

	for (auto& arg : node.getArguments()) {
		arg->accept(*this);
	}
	
	writer->endNode();
}

void Printer::visit(ast::FuncDeclaration& node)
{
	writer->startNode("func");
	printSignature(node);
	writer->endNode();
}

void Printer::visit(ast::FuncDefinition& node)
{
	writer->startNode("func");
	printSignature(node.getPrototype());

	writer->startNode("body");
	node.getBody().accept(*this);
	writer->endNode();
	writer->endNode();
}

void Printer::visit(ast::VariableDeclaration& node)
{
	writer->startNode("var");
	writer->writeValue("name", node.getName());
	writer->endNode();
}

void Printer::visit(ast::StatementBlock& node)
{
	writer->startNode("block");
	for (auto& stmt : node.getStatements()) {
		stmt->accept(*this);
	}
	writer->endNode();
}

void Printer::visit(ast::IfElseStatement& node)
{
	writer->startNode("if");
	
	writer->startNode("condition");
	node.getCondition().accept(*this);
	writer->endNode();

	writer->startNode("then");
	node.getThenBranch().accept(*this);
	writer->endNode();

	if (node.getElseBranch()) {
		writer->startNode("else");
		node.getElseBranch()->accept(*this);
		writer->endNode();
	}
	writer->endNode();
}

void Printer::visit(ast::NumberExpr& node)
{
	writer->startNode("number");
	writer->writeValue("value", node.getValue());
	writer->endNode();
}
void Printer::visit(ast::StringExpr& node)
{
	writer->startNode("string");
	writer->writeValue("value", node.getValue());
	writer->endNode();
}
void Printer::visit(ast::IdentifierExpr& node)
{
	writer->startNode("id");
	writer->writeValue("name", node.getName());
	writer->endNode();
}
void Printer::visit(ast::CallExpr& node)
{
	writer->startNode("call");
	writer->writeValue("func",node.getFunction());
	writer->startNode("arguments");
	for (auto& arg : node.getArguments()) {
		if (!arg)
			writer->startNode("asdasdasd");
		arg->accept(*this);
	}
	writer->endNode();	
	writer->endNode();	
}
void Printer::visit(ast::UnaryExpr& node)
{
	writer->startNode("unary");
	std::string const tmp = getTokenSpelling(TokenType(node.getOperation()));
	writer->writeValue("op", tmp);
	node.getOperand().accept(*this);
	writer->endNode();
}
void Printer::visit(ast::BinaryExpr& node)
{
	writer->startNode("binary");
	std::string const tmp = getTokenSpelling(TokenType(node.getOperation()));
	writer->writeValue("op", tmp);
	node.getLHS().accept(*this);
	node.getRHS().accept(*this);
	writer->endNode();
}
} // namespace hrscript
