/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_FileStream_
#define _hrscript_FileStream_
#include <string>
#include <fstream>

#include <hrscript/lexer/OctetStream.h>

namespace hrscript {
/*! Implements OctetStream as a file stream. */
class FileStream : public OctetStream {
public:
	typedef OctetStream::primitive_type primitive_type;

	FileStream(std::string filename)
		: pos(0), istream(filename, std::ios::binary | std::ios::ate)
	{
		size = istream.tellg();
		istream.seekg(0);
		
		extract();
	}

	FileStream() = delete;
	FileStream(FileStream&) = delete;

	/*! Get currently extracted octet. */
	virtual i32 getCurrent(primitive_type& out)
	{
		//fprintf(stderr,"%c",cur);
		return convert(cur, out);
	}
 
	/*! Peek at next octet */
	virtual i32 peek(primitive_type& out)
	{
		std::ifstream::int_type tmp;
		tmp = istream.peek();
		return convert(tmp, out);
	}

	/*! Extract next octet from stream. */
	virtual i32 getNext(primitive_type& out)
	{
		if (pos > size) {
			cur = 0;
			out = 0;
			return 0;
		}
		extract();
//#ifdef DEBUG
#if 0
		fprintf(stderr,"%c",cur);
		if(cur == '\n')
			printf("\n");
#endif // DEBUG
		return convert(cur, out);
	}

	/*! Get current position in the stream. */
	virtual size_t position() const
	{
		return pos;
	}
private:
	std::ifstream::int_type extract()
	{
		cur = istream.get();
		++pos;
	}

	i32 convert(std::ifstream::int_type in, primitive_type& out)
	{
		if (in == std::ifstream::traits_type::eof()) {
			out = 0;
			return 0;
		}

		out = in & 0xFF;
		return 1;		
	}

	primitive_type cur;
	size_t pos;
	size_t size;
	std::ifstream istream;
};
} // namespace hrscript
#endif//_hrscript_FileStream_
