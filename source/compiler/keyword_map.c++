/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <aw/script/lexer/keyword_map.h>
namespace aw {
namespace script {
// Link token to a keyword
keyword_map& keyword_map::add(std::string_view name, TokenType kind)
{
	_map[std::string(name)] = kind;
	return *this;
}

// Get token type from string
TokenType keyword_map::get(std::string_view name) const
{
	auto result = _map.find(name);

	if (result == std::end(_map))
		// TODO: better indication that it is not present
		return tok_illegal;

	return result->second;
}

// Check if keyword is present
bool keyword_map::is_keyword(std::string_view name) const
{
	return get(name) != tok_illegal;
}
} // namespace script
} // namespace aw
