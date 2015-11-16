/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <awengine/io/WriteStream.h>
#include <hrscript/lexer/FileStream.h>
#include <hrscript/lexer/Lexer.h>
#include <hrscript/parser/Parser.h>
#include <hrscript/utility/Printer.h>

namespace hrscript {
} // namespace hrscript

class TestWriteStream : public awrts::io::WriteStream {
public:
	virtual awrts::i32 put(char c)
	{
		std::cout.put(c);
		return 0;
	}

	virtual awrts::i32 put(std::string str)
	{
		std::cout << str;
		return 0;
	}

	virtual size_t position() const
	{
		return 0;
	}
};

int main(int argc, char** arg)
{
	using namespace hrscript;
	if (argc < 2)
		return 2;

	FileStream fstream(arg[1]);
	Lexer lexer(fstream);
	Parser parser(lexer);

	TestWriteStream out;
	Printer printer(out);
	
	auto decl = parser.parseDeclaration();
	while (decl) {
		decl->accept(printer);
		decl = parser.parseDeclaration();
	}
}
