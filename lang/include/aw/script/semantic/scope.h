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

#include <aw/script/ir/type.h>
#include <aw/script/ast/middle/declaration.h>

namespace aw::script {

enum scope_search_mode {
	current_scope,
	including_parents,
};

class scope {
public:
	using declaration = std::variant<
		middle::variable*,
		middle::function*
	>;

	explicit scope(scope* parent = nullptr)
		: parent_scope(parent)
	{

	}

#if 0
	scope(const scope& other, scope* parent = nullptr)
		: parent_scope(parent)
		, symbols(other.symbols)
	{

	}
#endif

	scope* parent() const { return parent_scope; }

	template<typename T>
	T* find(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents)
	{
		auto* sym = find_symbol(name, mode);
		if (auto ptr = get_if<T*>(sym))
			return *ptr;
		return nullptr;
	}

	ast::type* find_type(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents);

	middle::variable* find_var(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents)
	{
		return find<middle::variable>(name, mode);
	}

	middle::function* find_func(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents)
	{
		return find<middle::function>(name, mode);
	}

	void add_symbol(std::string_view name, declaration decl)
	{
		// TODO: handle conflicts
		symbols[name] = decl;
	}

	void add_type(std::string_view name, ast::type* type)
	{
		types[name] = type;
	}

protected:
	declaration* find_symbol(std::string_view name, scope_search_mode mode = scope_search_mode::including_parents);

private:

	scope* parent_scope = nullptr;

	std::unordered_map<std::string_view, declaration> symbols;
	std::unordered_map<std::string_view, ast::type*> types;
};

} // namespace aw::script

#endif // aw_script_scope_h
