/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <aw/types/string_view.h>
#include <aw/io/WriteStream.h>
#include <aw/script/diagnostic/diagnostics_engine.h>
#include <aw/script/lexer/source_buffer.h>
#include <aw/script/lexer/lexer.h>
#include <aw/script/parser/parser.h>
#include <aw/script/utility/printer.h>

namespace aw {
namespace script {
class TestWriteStream : public io::WriteStream {
public:
	virtual i32 put(char c)
	{
		std::cout.put(c);
		return 0;
	}

	virtual i32 put(std::string_view str)
	{
		std::cout << str;
		return 0;
	}

	virtual i32 write(const char* d, size_t s)
	{
		std::cout.write(d,s);
		return 0;
	}

	virtual size_t position() const
	{
		return 0;
	}
};
} // namespace script
} // namespace aw

int main(int argc, char** argv)
{
	using namespace aw;
	using namespace aw::script;
	if (argc < 2)
		return 2;

	io::read_file<io::file> file(argv[1]);
	source_buffer buffer(file);
	Lexer lexer(&buffer);
	diagnostics_engine diag;
	Parser parser(lexer, diag);

	TestWriteStream out;
	Printer printer(out);
	
	auto decl = parser.parseDeclaration();
	while (decl) {
		decl->accept(printer);
		decl = parser.parseDeclaration();
	}
}
