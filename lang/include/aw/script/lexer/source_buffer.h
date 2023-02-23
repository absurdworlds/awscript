/*
 * Copyright (C) 2016-2022 Hudd <haddayn@gmail.com>
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
	explicit source_buffer(std::string_view view)
		: buffer(view.data())
		, length(view.size())
	{

	}

	// TODO: Add support for mmap_file
	explicit source_buffer(aw::io::read_file<aw::io::file>& file)
		: source_buffer(read_file(file))
	{
	}

	source_buffer(source_buffer&& other) noexcept
		: buffer(other.buffer)
		, length(other.length)
		, owns_buffer(other.owns_buffer)
	{
		other.owns_buffer = false;
	}

	source_buffer& operator=(source_buffer&& other) noexcept
	{
		free();
		buffer = other.buffer;
		length = other.length;
		owns_buffer = other.owns_buffer;
		other.owns_buffer = false;
		return *this;
	}

	~source_buffer()
	{
		free();
	}

	char const* begin()
	{
		return buffer;
	}

	char const* end()
	{
		return buffer + length;
	}

	size_t size() const
	{
		return length;
	}

private:
	// TODO: a pair would be more idiomatic but it's private so whatever
	// and is going to be replaced with source_manager anyway
	std::string_view read_file(aw::io::read_file<aw::io::file>& file)
	{
		if (!file.is_open())
			return {};

		length = file.size();
		owns_buffer = true;

		auto buffer = new char[length + 1];

		auto res = file.read(buffer, length);

		if (res < 0)
			free();

		// Sentinel
		buffer[length] = 0;
		return std::string_view(buffer, length);
	}

	void free()
	{
		if (!owns_buffer)
			return;
		delete[] buffer;
		buffer = nullptr;
		length = 0;
	}

private:
	const char* buffer  = nullptr;
	size_t length = 0;
	bool owns_buffer = false;
};
} // namespace aw::script
#endif//aw_script_source_buffer_h
