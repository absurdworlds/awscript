/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_source_buffer_h
#define aw_script_source_buffer_h
#include <aw/io/read_file.h>
#include <aw/io/file.h>
namespace aw::script {
/*!
 * Wraps access to source files
 */
class source_buffer {
public:
	source_buffer(aw::io::read_file<aw::io::file>& file)
		: buffer(nullptr)
		, length(0)
	{
		if (!file.is_open())
			return;
		
		length = file.size();

		buffer = new char[length + 1];

		auto res = file.read(buffer, length);

		if (res < 0)
			free();

		// Sentinel
		buffer[length+1] = 0;
	}

	~source_buffer()
	{
	}

	char const* begin()
	{
		return buffer;
	}

	char const* end()
	{
		return buffer + length;
	}

	size_t size()
	{
		return length;
	}
private:
	void free()
	{
		if (buffer)
			delete[] buffer;
		buffer = 0;
		length = 0;
	}

	char* buffer;
	size_t length;
};
} // namespace aw::script
#endif//aw_script_source_buffer_h
