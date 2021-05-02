/*
 * Copyright (C) 2015-2021  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <aw/types/types.h>
#include <aw/algorithm/in.h>
#include <aw/script/lexer/lexer.h>

#include "char_type.h"
namespace aw {
namespace script {
Lexer::Lexer(source_buffer* inputBuffer)
	: buf(inputBuffer)
{
	// Setup keywords
	kwmap
	.add("const",      kw_const)
	.add("var",        kw_var)
	.add("func",       kw_func)
	.add("if",         kw_if)
	.add("else",       kw_else)
	.add("class",      kw_class)
	.add("prototype",  kw_prototype)
	.add("instance",   kw_instance)
	.add("return",     kw_return)
	.add("void",       kw_void)
	.add("float",      kw_float)
	.add("int",        kw_int)
	.add("string",     kw_string)
	.add("import",     kw_import)
	.add("export",     kw_export)
	.add("while",      kw_while)
	.add("do",         kw_do)
	.add("extern",     kw_extern)
	.add("break",      kw_break)
	.add("continue",   kw_continue);

	cur = buf->begin();
	end = buf->end();

	// Extract first token
	getNextToken();
}

void Lexer::init()
{
}

Token Lexer::getCurrentToken()
{
	return curToken;
}

/*!
 * Extract next token from stream
 */
Token Lexer::getNextToken()
{
	lexNextToken(curToken);
	return getCurrentToken();
}

bool Lexer::lex_identifier(Token& token)
{
	// TODO: support unicode
	char const* start = cur;
	cur = std::find_if_not(cur, end, is_name_char);

	std::string_view id(start, cur);

	// Check if token is a reserved keyword
	auto kind = kwmap.get(id);

	if (!kind)
		kind = tok_identifier;

	token = Token(kind, id, loc(start));
	
	return true;
}

bool Lexer::lex_numeric_constant(Token& token)
{
	char const* start = cur;

	// TODO: separate integer and real literals?
	// TODO: support short form: .05
	// TODO: "non-greedy" algorithm
	cur = std::find_if_not(cur, end, isalnum);

	if (*cur == '.') {
		cur = std::find_if_not(cur+1, end, isalnum);

		if (in(*cur, '-', '+') && in(prev(), 'e', 'E'))
			cur = std::find_if_not(cur, end, isalnum);
	}

	std::string_view num(start, cur);

	token = Token(tok_numeric_constant, num, loc(start));

	return true;
}

bool Lexer::lexStringLiteral(Token& token)
{
	char const* start = cur;
	// TODO: non-multiline strings
	while (*cur != '"') {
		cur = std::find(cur, end, '"');
		if (cur == end)
			break;
		if (prev() == '\\')
			++cur;
	}

	std::string_view str(start, cur);

	// TODO: incomplete flag if cur == end
	token = Token(tok_string_literal, str, loc(start));

	if (cur < end)
		++cur; // consume '"'

	return true;
}

bool Lexer::lexIllegalToken(Token& token)
{
	char const* begin = cur;
	// TODO: search until token-beginnning character
	cur = std::find_if(begin, end, isspace);

	std::string str(begin, cur);

	token = Token(tok_illegal, str, loc(begin));

	return true;
}

void Lexer::skip_line_comment()
{
	cur = std::find(cur, end, '\n');
}

void Lexer::skip_block_comment()
{
	// Skip ‘/* ’.
	constexpr ptrdiff_t skip_count = 3;
	// Third character:
	// - If it is '/', then it needs to be skipped or ‘/*/’
	// would be handled incorrectly
	// - If it is '*' followed by '/' then it is going to be handled
	// in the loop below (as a `prev()`)
	// - If it is anything else, then it doesn't matter and
	// can be skipped in any case.
	cur += std::min(skip_count, end - cur);

	while (true) {
		cur = std::find(cur, end, '/');

		if (cur == end)
			break;

		if (prev() == '*')
			break;
		++cur;
	}
}

void Lexer::handleComment()
{
	char p = peek();
	if (p == '*') {
		skip_block_comment();
	} else if (p == '/') {
		skip_line_comment();
	} else {
		// Not a comment - we're done.
		return;
	}
	
	// Instead of going through everything again, we do everything here.
	// Skip whitespace and check for more comments
	cur = std::find_if_not(cur+1, end, isspace);

	if (*cur == '/')
		handleComment();
}

char Lexer::peek() const
{
	return (cur < end) ? *(cur + 1) : 0;
}

char Lexer::prev() const
{
	assert(cur > std::begin(*buf));
	return *(cur - 1);
}

/*!
 * This function is an actual lexer implementation.
 * It reads characters from stream and produces tokens
 */
bool Lexer::lexNextToken(Token& tok)
{
	cur = std::find_if_not(cur, end, isspace);

lexNextToken:
	char const* tok_start = cur;

	auto location = loc(tok_start);

	switch (*cur) {
	case 0:
		tok = Token(tok_eof, location);
		return true;
	/* Numeric constants */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return lex_numeric_constant(tok);
	/* String literal */
	case '"':
		++ cur; // consume '"'
		return lexStringLiteral(tok);
	/* Identifier */
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		return lex_identifier(tok);
	/* Operators */
	case '^':
		tok = Token(tok_caret, location);
		break;
	case '~':
		tok = Token(tok_tilde, location);
		break;
	case ',':
		tok = Token(tok_comma, location);
		break;
	case '.':
		tok = Token(tok_dot, location);
		break;
	case ';':
		tok = Token(tok_semicolon, location);
		break;
	case '%':
		tok = Token(tok_percent, location);
		break;
	case '{':
		tok = Token(tok_l_brace, location);
		break;
	case '}':
		tok = Token(tok_r_brace, location);
		break;
	case '[':
		tok = Token(tok_l_square, location);
		break;
	case ']':
		tok = Token(tok_r_square, location);
		break;
	case '(':
		tok = Token(tok_l_paren, location);
		break;
	case ')':
		tok = Token(tok_r_paren, location);
		break;
	case '&':
		if (peek() == '&') {
			tok = Token(tok_amp_amp, location);
			++cur;
		} else {
			tok = Token(tok_amp, location);	
		}
		// TODO: does daedalus have '&=' operator?
		break;
	case '|':
		if (peek() == '|') {
			tok = Token(tok_pipe_pipe, location);
			++cur;
		} else {
			tok = Token(tok_pipe, location);
		}
		// TODO: does daedalus have '|= operator?
		break;
	case '!':
		if (peek() == '=') {
			tok = Token(tok_bang_equal, location);
			++cur;
		} else {
			tok = Token(tok_bang, location);
		}
		break;
	case '*':
		if (peek() == '=') {
			tok = Token(tok_ast_equal, location);
			++cur;
		} else {
			tok = Token(tok_ast, location);
		}
		break;
	case '/':
		// Look for comments first
		handleComment();

		// Check what we have, after we're done with comments
		// If we have '/', continue handling this case.
		// If we have something different, restart lexer.
		// TODO: I could've just restarted the lexer regardless,
		// is this optimization necessary?
		if (*cur != '/')
			// We didn't lex anything, restart the lexer.
			goto lexNextToken;

		if (peek() == '=') {
			tok = Token(tok_slash_equal, location);
			++cur;
		} else {
			tok = Token(tok_slash, location);
		}
		break;
	case '=':
		if (peek() == '=') {
			tok = Token(tok_equal_equal, location);
			++cur;
		} else {
			tok = Token(tok_equal, location);
		}
		break;
	case '+':
		if (peek() == '+') {
			tok = Token(tok_plus_plus, location);
			++cur;
		} else if (peek() == '=') {
			tok = Token(tok_plus_equal, location);
			++cur;
		} else {
			tok = Token(tok_plus, location);
		}
		break;
	case '-':
		if (peek() == '-') {
			tok = Token(tok_minus_minus, location);
			++cur;
		} else if (peek() == '=') {
			tok = Token(tok_minus_equal, location);
			++cur;
		} else {
			tok = Token(tok_minus, location);
		}
		break;
	case '<':
		if (peek() == '<') {
			tok = Token(tok_less_less, location);
			++cur;
		} else if (peek() == '=') {
			tok = Token(tok_less_equal, location);
			++cur;
		} else {
			tok = Token(tok_less, location);
		}
		break;
	case '>':
		if (peek() == '>') {
			tok = Token(tok_greater_greater, location);
			++cur;
		} else if (peek() == '=') {
			tok = Token(tok_less_equal, location);
			++cur;
		} else {
			tok = Token(tok_less, location);
		}
		break;
	/* Illegal token */
	default:
		// TODO: handle unicode
		return lexIllegalToken(tok);
	}

	++cur;
	std::string_view val(tok_start, cur);
	tok.set_data(val);

	return true;
}
} // namespace script
} // namespace aw
