/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_lexer_
#define _daedalus_lexer_
#include <daedalus/lexer/OctetStream.h>
#include <daedalus/lexer/Token.h>
namespace deadalus {
class Lexer {
public:
	Lexer(OctetStream& stream)
		: stream(stream)
	{
	}

	void init ();
	Token getCurrentToken();
	Token getNextToken();

private:
	Token curToken;

	OctetStream& stream;
};
} // namespace daedalus
#endif//_daedalus_lexer_
