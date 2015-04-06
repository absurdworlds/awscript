/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_ClassDeclaration_
#define _daedalus_AST_ClassDeclaration_
#include <daedalus/ast/Declaration.h>
#include <daedalus/ast/Statement.h>
namespace daedalus {
/* TODO: give this class a more suitable name
 * (prototype is not a new type, but rather just a template for
 * an instance, or at least it seemes to be so)
 */
class TypeDeclaration : public Declaration {
	virtual ~TypeDeclaration();

	virtual accept(ast::Visitor& visitor) = 0;
}

class ClassDeclaration : public TypeDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
private:
	std::vector<Declaration*> body;
};

class PrototypeDeclaration : public TypeDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
protected:
	TypeDeclaration* base;
	// It seems like prototype allows only assignments inside it,
	// while instance additionally allows at least function calls
	std::vector<Statement*> stmts;
};

class InstanceDeclaration : public Declaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
private:
	TypeDeclaration* base;
	std::vector<Statement*> stmts;
};
} // namespace daedalus
#endif//_daedalus_AST_ClassDeclaration_
