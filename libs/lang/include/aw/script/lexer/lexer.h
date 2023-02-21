/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_lang_lexer_h
#define aw_script_lang_lexer_h

#include <aw/script/lexer/source_buffer.h>
#include <aw/script/lexer/token.h>

namespace aw::script {

class lexer {
public:
	// TODO: not_null
	lexer(source_buffer* buffer);
	~lexer();

	token current() const;
	token next();

private:
	location make_location(const char* pos) const;

	bool lex(token& tok);

	bool lex_eof(token& tok) const;
	bool lex_numeric_constant(token& token);
	bool lex_string_literal(token& token);
	bool lex_identifier(token& token);
	bool lex_operator(token& token);
	bool lex_comment(token& token);
	bool lex_line_comment(token& token);
	bool lex_block_comment(token& token);
	bool lex_illegal_token(token& token);

	char prev() const;
	char peek() const;


private:
	token cur_token;
	source_buffer* buf;

	char const* cur = nullptr;
	char const* end = nullptr;
};

} // namespace aw::script
#endif // aw_script_lang_lexer_h
