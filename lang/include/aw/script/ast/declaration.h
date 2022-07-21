/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_ast_decl_h
#define aw_script_ast_decl_h

#include <cassert>

#include <aw/types/string_view.h>

namespace aw::script::ast {

enum class decl_kind {
	type,
	alias_type,
	class_type,
	function,
	variable,
};

class declaration {
protected:
	explicit declaration(decl_kind kind, string_view name)
		: _kind(kind), _name(name)
	{}

	declaration& operator=(const declaration& other)
	{
		assert(_kind == other.kind());
		_name = other.name();
		return *this;
	}

	declaration& operator=(declaration&& other) noexcept
	{
		assert(_kind == other.kind());
		_name = other.name();
		return *this;
	}

public:
	declaration(const declaration& other) = delete;
	declaration(declaration&& other) = delete;

	virtual ~declaration() = default;

	decl_kind   kind() const { return _kind; }
	string_view name() const { return _name; }

private:
	decl_kind   _kind;
	string_view _name;
};

} // namespace aw::script::ast

#endif//aw_script_ast_decl_h
