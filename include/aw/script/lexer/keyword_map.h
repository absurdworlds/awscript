/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_keyword_map_h
#define aw_script_keyword_map_h
#include <map>
#include <string>
#include <aw/script/lexer/token.h>

namespace aw::script {
/*!
 * Helper class for keyword lookup
 */
class keyword_map {
public:
	/*!
	 * Add a symbol to keyword map.
	 * (Link string to a token)
	*/
	keyword_map& add(std::string def, TokenType kind);

	/*!
	 * Get token type from string
	 */ 
	TokenType get(std::string def);

	/*!
	 * Check if string is a keyword.
	 * Checks if it is present in the map.
	 */
	bool is_keyword(std::string def);

private:
	std::map<std::string, TokenType> theMap;
};
} // namespace aw::script
#endif//aw_script_keyword_map_h
