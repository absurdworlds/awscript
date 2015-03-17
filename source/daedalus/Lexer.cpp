/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/common/types.h>
#include <daedalus/lexer/Lexer.h>
namespace daedalus {

void Lexer::init ()
{
}

Token Lexer::getCurrentToken()
{
	return curToken;
}

/*! Temporary (TODO) helper function to recognize identifier */
void processIdentifier(Token& identifier)
{
	std::string const id = identifier.getData();
	if (id == "const")
		identifier.setType(KEYWORD(const));
	else if (id == "var")
		identifier.setType(KEYWORD(var));
	else if (id == "func")
		identifier.setType(KEYWORD(func));
	else if (id == "if")
		identifier.setType(KEYWORD(if));
	else if (id == "else")
		identifier.setType(KEYWORD(else));
	else if (id == "class")
		identifier.setType(KEYWORD(class));
	else if (id == "prototype")
		identifier.setType(KEYWORD(prototype));
	else if (id == "instance")
		identifier.setType(KEYWORD(instance));
	else if (id == "return")
		identifier.setType(KEYWORD(return));
	else if (id == "void")
		identifier.setType(KEYWORD(void));
	else if (id == "float")
		identifier.setType(KEYWORD(float));
	else if (id == "int")
		identifier.setType(KEYWORD(int));
	else if (id == "string")
		identifier.setType(KEYWORD(string));
	else
		identifier.setType(TOKEN(identifier));
}

Token Lexer::lexIdentifier()
{
	Token tok;

	char c;
	std::string id(1, c);

	stream.getCurrent(c);
	while (isalnum(c) || c == '_') {
		id += c;
		stream.getNext(c);
	}

	tok.setData(id);

	//FIXME: use map
	processIdentifier(tok);
	
	return tok;
}

#if 0 // started write it for the second time
      // am I falling into dementia?
Token Lexer::lexIdentifier()
{
	std::string str;
	char c;
	char prev;
	stream.getCurrent(c);

	while (isalnum(c) || c == '_') {
		num += c;
		stream.getNext(c);
	}

	Token token;
	token.setData(str);
	token.setType(tok_identifier);

	return token;
}
#endif

Token Lexer::lexNumericConstant()
{
	std::string num;
	char c;
	char prev;
	stream.getCurrent(c);

	while (isalnum(c) || c == '.') {
		num += c;
		prev = c;
		stream.getNext(c);
	}

	if ((c == '-' || c == '+') && (prev == 'e' || prev == 'E')) {
		do {
			num += c;
			stream.getNext(c);
		} while (isalnum(c) || c == '.');
	}

	Token token;
	token.setData(num);
	token.setType(tok_numeric_constant);

	return token;
}

Token Lexer::lexStringLiteral()
{
	Token token;
	std::string str;
	char c;
	stream.getCurrent(c);

	while (c != '"') {
		if (c == '\\') {
			stream.getNext(c);
		}
		str += c;
		stream.getNext(c);
	}

	stream.getNext(c); // consume '"'

	token.setData(str);
	token.setType(tok_string_literal);

	return token;
}

Token Lexer::lexIllegalToken()
{
	std::string str;
	char c;

	while (!isspace(c)) {
		str += c;
		stream.getNext(c);
	}

	Token token;
	token.setData(str);
	token.setType(tok_illegal);

	return token;
}

/*! Extract next token from stream */
Token Lexer::getNextToken()
{
	Token tok;

	char c;
	stream.getCurrent(c);

	while(isspace(c))
		stream.getNext(c);

	switch (c) {
	case 0:
		tok.setType(tok_eof);
		break;
	/* Numeric constants */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		tok = lexNumericConstant();
		break;
	/* String literal */
	case '"':
		stream.getNext(c); // consume '"'
		tok = lexStringLiteral();
		break;
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
		tok = lexIdentifier();
		break;
	/* Operators */
	case '^':
		tok.setType(tok_caret);
		break;
	case '~':
		tok.setType(tok_tilde);
		break;
	case ',':
		tok.setType(tok_comma);
		break;
	case '.':
		tok.setType(tok_dot);
		break;
	case '%':
		tok.setType(tok_percent);
		break;
	case '{':
		tok.setType(tok_l_brace);
		break;
	case '}':
		tok.setType(tok_r_brace);
		break;
	case '[':
		tok.setType(tok_l_square);
		break;
	case ']':
		tok.setType(tok_r_square);
		break;
	case '(':
		tok.setType(tok_l_paren);
		break;
	case ')':
		tok.setType(tok_r_paren);
		break;
	case '&':
		stream.getNext(c);
		if (c == '&') {
			tok.setType(tok_amp_amp);
		} else {
			tok.setType(tok_amp);	
		}
		// TODO: does daedalus have '&=' operator?
		break;
	case '|':
		stream.getNext(c);
		if (c == '|') {
			tok.setType(tok_pipe_pipe);
		} else {
			tok.setType(tok_pipe);
		}
		// TODO: does daedalus have '|= operator?
	case '!':
		stream.getNext(c);
		if (c == '=') {
			tok.setType(tok_bang_equal);
		} else {
			tok.setType(tok_bang);
		}
		break;
	case '*':
		stream.getNext(c);
		if (c == '=') {
			tok.setType(tok_ast_equal);
		} else {
			tok.setType(tok_ast);
		}
		break;
	case '/':
		stream.getNext(c);
		if (c == '=') {
			tok.setType(tok_slash_equal);
		} else {
			tok.setType(tok_slash);
		}
		break;
	case '=':
		stream.getNext(c);
		if (c == '=') {
			tok.setType(tok_equal_equal);
		} else {
			tok.setType(tok_equal);
		}
		break;
	case '+':
		stream.getNext(c);
		if (c == '+') {
			tok.setType(tok_plus_plus);
		} else if (c == '=') {
			tok.setType(tok_plus_equal);
		} else {
			tok.setType(tok_plus);
		}
		break;
	case '-':
		stream.getNext(c);
		if (c == '-') {
			tok.setType(tok_minus_minus);
		} else if (c == '=') {
			tok.setType(tok_minus_equal);
		} else {
			tok.setType(tok_minus);
		}
		break;
	case '<':
		stream.getNext(c);
		if (c == '<') {
			tok.setType(tok_less_less);
		} else if (c == '=') {
			tok.setType(tok_less_equal);
		} else {
			tok.setType(tok_less);
		}
		break;
	case '>':
		stream.getNext(c);
		if (c == '>') {
			tok.setType(tok_greater_greater);
		} else if (c == '=') {
			tok.setType(tok_less_equal);
		} else {
			tok.setType(tok_less);
		}
		break;
	/* Illegal token */
	default:
		tok = lexIllegalToken();
		break;
	}

	curToken = tok;
	return curToken;
}
} // namespace daedalus
