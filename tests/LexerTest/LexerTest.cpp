/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cstdio>
#include <aw/io/ReadFile.h>
#include <aw/io/InputFileStream.h>
#include <hrscript/lexer/Lexer.h>
#include <hrscript/utility/PrintToken.h>
namespace aw {
namespace script {
void printTokenInfo(Token& token)
{
	std::string msg;

	bool show_data = token == tok_identifier       ||
	                 token == tok_numeric_constant ||
	                 token == tok_string_literal;

	if(show_data) {
		msg += getTokenName(token.getType());
		msg += ", ";
		msg += token.getData();
	} else {
		msg += getTokenName(token.getType());
	}

	printf("〈%s〉 ", msg.c_str());
}
} // namespace script
} // namespace aw

int main(int argc, char** arg)
{
	using namespace aw::io;
	using namespace aw::script;
	if (argc < 2)
		return 2;

	InputFileStream fstream(arg[1]);
	Lexer lexer(fstream);

	Token token = lexer.getCurrentToken();
	do {
		printTokenInfo(token);
		token = lexer.getNextToken();
	} while (token.getType() != tok_eof);
}
