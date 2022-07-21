/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_token_h
#define aw_script_token_h
#include <aw/types/string_view.h>
#include <aw/script/utility/location.h>
namespace aw::script {
enum class token_kind {
#define TOKEN(x) x,
#define PUNCT(x, y) TOKEN(x)
#define KEYWORD(x) kw_ ## x,
#include <aw/script/lexer/tokens.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
};

struct token {
	token_kind  kind = token_kind::none;
	string_view data;
	location    loc;
	bool        incomplete = false;
};

/*! Returns true if token is an identifier */
inline bool is_identifier(token tok)
{
	return tok.kind == token_kind::identifier;
}

inline bool operator==(token tok, token_kind kind)
{
	return tok.kind == kind;
}
} // namespace aw::script
#endif//aw_script_token_h
