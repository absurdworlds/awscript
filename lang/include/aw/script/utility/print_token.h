/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_print_token_h
#define aw_script_print_token_h
#include <string>
#include <aw/script/lexer/token.h>
#include <aw/meta/pp/macro.h>
namespace aw {
namespace script {
/*!
 * Print token name (as in source code)
 */
inline std::string name_token(token_kind type);

/*!
 * Print sequence of characters which would produce the token
 */
inline std::string spell_token(token_kind kind);

/*!
 * Get characters compirising the token.
 */
inline std::string spell_token(token token)
{
	return std::string(token.data);
}

std::string name_token(token_kind type)
{
	switch (type) {
// some macro magic to print token names
#define TOKEN(x)    case token_kind::x: return AW_TO_STR(x);
#define PUNCT(x, y) case token_kind::x: return AW_TO_STR(x);
#define KEYWORD(x)  case token_kind::kw_##x: return AW_TO_STR(x);
#include <aw/script/lexer/tokens.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
	return {};
}

std::string spell_token(token_kind kind)
{
	switch (kind) {
#define TOKEN(x)    case token_kind::x: return AW_TO_STR(x);
#define PUNCT(x, y) case token_kind::x: return y;
#define KEYWORD(x)  case token_kind::kw_##x: return AW_TO_STR(x);
#include <aw/script/lexer/tokens.h>
#undef TOKEN
#undef PUNCT
#undef KEYWORD
	}
	return {};
}

#undef TOKEN1
#undef KEYWORD1
} // namespace script
} // namespace aw
#endif//aw_script_print_token_h
