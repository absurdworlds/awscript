/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
namespace deadalus {
#define TOKEN(x) tok_ ## x
#define PUNCT(x y) TOKEN(x)
#define KEYWORD(x) kw_ ## x
enum TokenType {
	TOKEN(illegal),
	TOKEN(eof),
	TOKEN(identifier),
	PUNCT(comma,            ","),
	PUNCT(dot,              "."),
	PUNCT(l_brace,          "{"),
	PUNCT(r_brace,          "}"),
	PUNCT(l_square,         "["),
	PUNCT(r_square,         "]"),
	PUNCT(l_paren,          "("),
	PUNCT(r_paren,          ")"),
	PUNCT(amp,              "&"),
	PUNCT(amp_amp,          "&&"),
	PUNCT(pipe,             "|"),
	PUNCT(pipe_pipe,        "||"),
	PUNCT(ast,              "*"),
	PUNCT(ast_equal,        "*="),
	PUNCT(plus,	        "+"),
	PUNCT(plus_equal,       "+="),
	PUNCT(plus_plus,        "++"),
	PUNCT(minus,	        "-"),
	PUNCT(minus_equal,      "-="),
	PUNCT(minus_minus,      "--"),
	PUNCT(slash,	        "/"),
	PUNCT(slash_equal,      "/="),
	PUNCT(percent,	        "%"),
	PUNCT(less,	        "<"),
	PUNCT(less_equal,       "<="),
	PUNCT(less_less,        "<<"),
	PUNCT(greater,          ">"),
	PUNCT(greater_equal,    ">="),
	PUNCT(greater_greater,  ">>"),
	PUNCT(exclamation,      "!"),
	PUNCT(tilde,            "~"),
	PUNCT(caret,            "^"),
	PUNCT(equal,            "="),
	PUNCT(equal_equal,      "=="),
	KEYWORD(const),
	KEYWORD(var),
	KEYWORD(func),
	KEYWORD(if),
	KEYWORD(else),
	KEYWORD(class),
	KEYWORD(prototype),
	KEYWORD(instance),
	KEYWORD(return),
	KEYWORD(void),
	KEYWORD(float),
	KEYWORD(int),
	KEYWORD(string),
};

class Token {
public:
	TokenType getType() const;
	void setType(TokenType newType)
	{
		type = type;
	}

	std::string getData() const
	{
		return data;
	}

	void setData(std::string newData)
	{
		data = newData;
	}
private:
	TokenType type;
	std::string data;
#if 0
	char* dataPtr;
	size_t dataSize;
#endif
}
} // namespace daedalus
