/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
namespace deadalus {
#define TOKEN(x) x
#define PUNCT(x y) p_ ## x
#define KEYWORD(x) kw_ ## x
enum TokenType {
	TOKEN(eof),
	TOKEN(identifier),
	PUNCT(comma,		","),
	PUNCT(dot,		"."),
	PUNCT(l_brace,		"{"),
	PUNCT(r_brace,		"}"),
	PUNCT(l_square,		"["),
	PUNCT(r_square,		"]"),
	PUNCT(l_paren,		"("),
	PUNCT(r_paren,		")"),
	PUNCT(amp,		"&"),
	PUNCT(damp,		"&&"),
	PUNCT(pipe,		"|"),
	PUNCT(dpipe,		"||"),
	PUNCT(ast,		"*"),
	PUNCT(ast_equal,	"*="),
	PUNCT(plus,		"+"),
	PUNCT(plus_equal,	"+="),
	PUNCT(minus,		"-"),
	PUNCT(minus_equal,	"-="),
	PUNCT(slash,		"/"),
	PUNCT(slash_equal,	"/="),
	PUNCT(percent,		"%"),
	PUNCT(less,		"<"),
	PUNCT(less_equal,	"<="),
	PUNCT(dless,		"<<"),
	PUNCT(greater,		">"),
	PUNCT(greater_equal,	">="),
	PUNCT(dgreater,		">>"),
	PUNCT(exclam,		"!"),
	PUNCT(tilde,		"~"),
	PUNCT(caret,		"^"),
	PUNCT(equal,		"="),
	PUNCT(dequal,		"=="),
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
	void setType(TokenType type)
	{
		type_ = type;
	}
private:
	TokenType type_;
	
}
} // namespace daedalus
