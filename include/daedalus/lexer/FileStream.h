/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_FileStream_
#define _daedalus_FileStream_
#include <string>
#include <fstream>

#include <daedalus/lexer/OctetStream.h>

namespace daedalus {
/*! Implements OctetStream as a file stream. */
class FileStream : public OctetStream {
public:
	typedef OctetStream::primitive_type primitive_type;

	FileStream(std::string filename)
		: pos(0)
	{
	}

	FileStream() = delete;
	FileStream(FileStream&) = delete;

	/*! Get currently extracted octet. */
	virtual i32 getCurrent(primitive_type& out)
	{
		return 0;
	}

	/*! Extract next octet from stream. */
	virtual i32 getNext(primitive_type& out)
	{
		return 0;
	}

	/*! Get current position in the stream. */
	virtual size_t position() const
	{
		return 0;
	}
private:
	size_t pos;
	std::ifstream istream;
};
} // namespace daedalus
#endif//_daedalus_FileStream_
