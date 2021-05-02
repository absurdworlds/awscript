/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <aw/script/ast/decl/variable.h>
#include <aw/script/ast/decl/function.h>

#include <aw/script/ast/expr/unary.h>
#include <aw/script/ast/expr/binary.h>
#include <aw/script/ast/expr/call.h>
#include <aw/script/ast/expr/number.h>
#include <aw/script/ast/expr/string.h>
#include <aw/script/ast/expr/identifier.h>

#include <aw/script/ast/if_else_statement.h>
#include <aw/script/ast/statement_block.h>

#include <aw/script/utility/printer.h>
#include <aw/script/utility/print_token.h>

namespace aw {
namespace script {
Printer::Printer(aw::io::WriteStream& out)
	: writer(out)
{
}

void Printer::write_value(std::string_view name, std::string_view str)
{
	writer.write_value(name, doc::value(str));
}

void Printer::printSignature(ast::FunctionProto& node)
{
	write_value("name",   node.getName());
	write_value("return", node.getReturnType());

	writer.start_node("arguments");

	for (auto& arg : node.getArguments()) {
		arg->accept(*this);
	}
	
	writer.end_node();
}

void Printer::visit(ast::FunctionProto& node)
{
	writer.start_node("func");
	printSignature(node);
	writer.end_node();
}

void Printer::visit(ast::Function& node)
{
	writer.start_node("func");
	printSignature(node.getPrototype());

	writer.start_node("body");
	node.getBody().accept(*this);
	writer.end_node();
	writer.end_node();
}

void Printer::visit(ast::Variable& node)
{
	writer.start_node("var");
	write_value("name", node.getName());
	writer.end_node();
}

void Printer::visit(ast::StatementBlock& node)
{
	writer.start_node("block");
	for (auto& stmt : node.getStatements()) {
		stmt->accept(*this);
	}
	writer.end_node();
}

void Printer::visit(ast::IfElseStatement& node)
{
	writer.start_node("if");
	
	writer.start_node("condition");
	node.getCondition().accept(*this);
	writer.end_node();

	writer.start_node("then");
	node.getThenBranch().accept(*this);
	writer.end_node();

	if (node.getElseBranch()) {
		writer.start_node("else");
		node.getElseBranch()->accept(*this);
		writer.end_node();
	}
	writer.end_node();
}

void Printer::visit(ast::NumberExpr& node)
{
	writer.start_node("number");
	write_value("value", node.getValue());
	writer.end_node();
}
void Printer::visit(ast::StringExpr& node)
{
	writer.start_node("string");
	write_value("value", node.getValue());
	writer.end_node();
}
void Printer::visit(ast::IdentifierExpr& node)
{
	writer.start_node("id");
	write_value("name", node.getName());
	writer.end_node();
}
void Printer::visit(ast::CallExpr& node)
{
	writer.start_node("call");
	write_value("func",node.getFunction());
	writer.start_node("arguments");
	for (auto& arg : node.getArguments()) {
		if (!arg)
			writer.start_node("asdasdasd");
		arg->accept(*this);
	}
	writer.end_node();
	writer.end_node();
}
void Printer::visit(ast::UnaryExpr& node)
{
	writer.start_node("unary");
	std::string const tmp = spellToken(TokenType(node.getOperation()));
	write_value("op", tmp);
	node.getOperand().accept(*this);
	writer.end_node();
}

void Printer::visit(ast::BinaryExpr& node)
{
	writer.start_node("binary");
	std::string const tmp = spellToken(TokenType(node.getOperation()));
	write_value("op", tmp);
	node.getLHS().accept(*this);
	node.getRHS().accept(*this);
	writer.end_node();
}
} // namespace script
} // namespace aw
