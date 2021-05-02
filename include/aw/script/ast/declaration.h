/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_declaration_h
#define aw_script_ast_declaration_h
#include <aw/script/ast/node.h>

namespace aw {
namespace script {
namespace ast {
/*!
 * Declaration class represents a declaration or definition,
 * such as class declaration, function declaration etc.
 */
class Declaration {
public:
	virtual ~Declaration() = default;

	//! Implements Visitor Pattern
	virtual void accept(ast::Visitor& visitor) = 0;


	//! List of concrete derived types
	enum Kind {
		FunctionProto,
		Function,
		Variable,
		Class,
	};

	Kind getKind() const
	{
		return kind;
	}

protected:
	Declaration(Kind k)
		: kind(k)
	{
	}

private:
	Kind kind;
};

} // namespace ast
} // namespace script
} // namespace aw
#endif//aw_script_ast_declaration_h
