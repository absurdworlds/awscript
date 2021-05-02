/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_KeywordMap_
#define hrscript_KeywordMap_
#include <map>
#include <string>
#include <hrscript/lexer/Token.h>

namespace aw {
namespace script {
/*!
 * Helper class for keyword lookup
 */
class KeywordMap {
public:
	/*!
	 * Add a symbol to keyword map.
	 * (Link string to a token)
	*/
	KeywordMap& add(std::string def, TokenType kind);
	/*!
	 * Get token type from string
	 */ 
	TokenType get(std::string def);
	/*!
	 * Check if string is a keyword.
	 * Checks if it is present in the map.
	 */
	bool isKeyword(std::string def);
private:
	std::map<std::string, TokenType> theMap;
};
} // namespace hrscript
} // namespace aw
#endif//hrscript_KeywordMap_
