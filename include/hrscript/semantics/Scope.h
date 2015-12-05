/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_scope_h
#define hrscript_scope_h
namespace hrscript {

/*!
 * The Scope class represents an environment where different names 
 * may be defined. This class is used to resolve name to its declaration.
 */
class Scope {
public:
	Scope(Scope* parent)
		: parent(parent)
	{
	}

	Declaration* getSymbol(std::string name);
private:
	// Parent scope
	Scope* parent;

};
} // namespace hrscript
#endif//hrscript_scope_h
