/*
 * Copyright (C) 2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_scope_h
#define aw_script_scope_h

#include <string_view>
#include <unordered_map>

namespace aw::script::ast {
class declaration;

enum scope_search_mode {
	current_scope,
	including_parents,
};

class scope {
public:
	explicit scope(scope* parent = nullptr)
		: parent_scope(parent)
	{

	}

	scope* parent() const { return parent_scope; }

	declaration* find_symbol(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents);

	void add_symbol(std::string_view name, declaration* decl)
	{
		// TODO: handle conflicts
		symbols[name] = decl;
	}

private:
	scope* parent_scope = nullptr;

	std::unordered_map<std::string_view, declaration*> symbols;
};

} // namespace aw::script::ast

#endif // aw_script_scope_h
