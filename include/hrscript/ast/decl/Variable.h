/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_AST_decl_Variable_
#define hrscript_AST_decl_Variable_
#include <memory>
#include <string>
#include <hrscript/ast/Declaration.h>
#include <hrscript/ast/Expression.h>
namespace hrscript {
namespace ast {
class Variable : public Declaration {
public:
	Variable(std::string id)
		: name(id), writeAccessible(true)
	{
	}

	virtual ~Variable() = default;

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	virtual Expression* getInitializer()
	{
		return val.get();
	}

	virtual bool isWriteable(bool writeable)
	{
		return writeAccessible;
	}

	virtual void setWriteable(bool writeable)
	{
		writeAccessible = writeable;
	}

	virtual void setInitialier(std::unique_ptr<Expression> newVal)
	{
		val = std::move(newVal);
	}

	std::string getName() const
	{
		return name;
	}
private:
	std::string name;
	std::unique_ptr<Expression> val;
	bool writeAccessible;
};
} // namespace ast
} // namespace hrscript
#endif//hrscript_AST_decl_Variable_
