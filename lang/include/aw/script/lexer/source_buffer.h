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
#include <aw/io/file.h>
#include <aw/io/read_file.h>
#include <aw/types/char_buffer.h>
namespace aw::script {
/*!
 * Wraps access to source files
 */
class source_buffer {
public:
	explicit source_buffer(std::string_view view)
		: buffer(view)
	{

	}

	// TODO: Add support for mmap_file
	explicit source_buffer(aw::io::read_file<aw::io::file>& file)
		: source_buffer(read_file(file))
	{
	}

	source_buffer(source_buffer&& other) noexcept = default;

	source_buffer& operator=(source_buffer&& other) noexcept = default;

	~source_buffer()
	{
	}

	char const* begin()
	{
		return buffer.begin();
	}

	char const* end()
	{
		return buffer.end();
	}

	size_t size() const
	{
		return buffer.view_size();
	}

private:
	struct buffer_data {
		char* ptr = nullptr;
		size_t size = 0;
		size_t view_length = size;
	};

	explicit source_buffer(buffer_data data)
		: buffer(data.ptr, data.size, data.view_length)
	{
	}

	static auto read_file(aw::io::read_file<aw::io::file>& file) -> buffer_data
	{
		if (!file.is_open())
			return {};

		auto length = file.size();

		std::unique_ptr<char[]> buffer(new char[length + 1]);

		auto res = file.read(buffer.get(), length);

		if (res < 0)
			buffer.reset();

		// Sentinel
		buffer[length] = 0;

		return buffer_data{
			.ptr = buffer.release(),
			.size = length + 1,
			.view_length = length,
		};
	}

	char_buffer buffer;
};
} // namespace aw::script
#endif//aw_script_source_buffer_h
