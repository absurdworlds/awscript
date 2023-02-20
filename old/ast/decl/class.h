/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_class_declaration_h
#define aw_script_ast_class_declaration_h
#include <aw/script/ast/decl/variable.h>
namespace aw::script {
namespace ast {
class Variable;
class ClassDeclaration : public Declaration {
public:
	ClassDeclaration(std::string_view name)
		: Declaration(Class)
		, _name(name)
	{
	}
	ClassDeclaration(std::string_view name, std::vector<uptr<ast::Variable>> members)
		: Declaration(Class)
		, _name(name)
		, members(std::move(members))
	{
	}


	virtual ~ClassDeclaration() = default;

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::string_view name() const { return _name; }

	std::vector<uptr<ast::Variable>> const& data_members() const
	{
		return members;
	}

private:
	std::string_view _name;
	std::vector<uptr<ast::Variable>> members;
};
} // namespace ast
} // namespace aw::script
#endif//aw_script_ast_class_declaration_h
