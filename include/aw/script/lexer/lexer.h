/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_lexer_h
#define aw_script_lexer_h
#include <aw/script/lexer/source_buffer.h>
#include <aw/script/lexer/token.h>
#include <aw/script/lexer/keyword_map.h>
namespace aw {
namespace script {
class Lexer {
public:
	Lexer(source_buffer* inputBuffer);

	Token getCurrentToken();
	Token getNextToken();

private: //func
	void init();
	char peek();

	bool lexIllegalToken(Token& token);
	bool lex_identifier(Token& token);
	bool lexStringLiteral(Token& token);
	bool lexNumericConstant(Token& token);
	bool lexNextToken(Token& token);
	void skipLineComment();
	void skipBlockComment();
	void handleComment();

private: //data
	Token curToken;
	keyword_map kwmap;
	source_buffer* buf;

	char const* cur = nullptr;
	char const* end = nullptr;
};
} // namespace script
} // namespace aw
#endif//aw_script_lexer_h
