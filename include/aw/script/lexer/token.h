/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_token_h
#define aw_script_token_h
#include <string>
namespace aw {
namespace script {
struct location {
	location() = default;
	location(size_t offset)
		: pos(offset)
	{
	}

	unsigned fileId = 0;
	size_t   pos = 0;
};

enum TokenType {
#define TOKEN(x) tok_ ## x,
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x,
#include <aw/script/lexer/tokens.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
};

class Token {
public:
	Token()
		: _kind(tok_illegal)
	{}

	Token(TokenType kind)
		: _kind(kind)
	{}

	Token(TokenType kind, location loc)
		: _kind(kind), _loc(loc)
	{}

	Token(TokenType kind, std::string_view data, location loc)
		: _kind(kind), _data(data), _loc(loc)
	{
	}

	TokenType type() const
	{
		return _kind;
	}
	
	std::string_view data() const
	{
		return _data;
	}

	void set_data(std::string_view data)
	{
		_data = data;
	}

	script::location location() const
	{
		return _loc;
	}

private:
	TokenType _kind;

	std::string_view _data;
	script::location _loc;
};

/*! Returns true if token is an identifier */
inline bool is_identifier(Token tok)
{
	return tok.type() == tok_identifier;
}

/*!
 * Returns true if token is a type name (in other words, can appear
 * after ‘var’)
 */
inline bool is_type_name(Token tok)
{
	return tok.type() == kw_func ||
	       tok.type() >= kw_void ||
	       is_identifier(tok);
}

inline bool operator==(Token tok, TokenType kind)
{
	return tok.type() == kind;
}
} // namespace script
} // namespace aw
#endif//aw_script_token_h
