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
namespace daedalus {
class ClassDeclaration : public Declaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
protected:
	std::vector<Declaration*> body;
};

class PrototypeDeclaration : public ClassDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
protected:
	// It seems like prototype allows only assignments inside it,
	// while instance additionally allows at least function calls
	std::vector<Statement*> stmts;
};

class InstanceDeclaration : public PrototypeDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual accept(ast::Visitor& visitor);
};
} // namespace daedalus
#endif//_daedalus_AST_ClassDeclaration_
