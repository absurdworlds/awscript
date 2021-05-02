/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cstdio>
#include <aw/io/file.h>
#include <aw/io/read_file.h>
#include <aw/script/lexer/lexer.h>
#include <aw/script/utility/print_token.h>
namespace aw {
namespace script {
void printTokenInfo(Token& token)
{
	std::string msg;

	bool show_data = token == tok_identifier       ||
	                 token == tok_numeric_constant ||
	                 token == tok_string_literal;

	if(show_data) {
		msg += nameToken(token.type());
		msg += ", ";
		msg += token.data();
	} else {
		msg += nameToken(token.type());
	}

	printf("〈%s〉 ", msg.c_str());
}
} // namespace script
} // namespace aw

int main(int argc, char** arg)
{
	using namespace aw;
	using namespace aw::script;
	if (argc < 2)
		return 2;

	io::read_file<io::file> file(arg[1]);
	source_buffer buffer(file);
	Lexer lexer(&buffer);

	Token token = lexer.getCurrentToken();
	do {
		printTokenInfo(token);
		token = lexer.getNextToken();
	} while (token.type() != tok_eof);
}
