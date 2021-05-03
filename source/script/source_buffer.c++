/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <aw/script/utility/location.h>
#include <aw/script/lexer/source_buffer.h>

namespace aw {
namespace script {
source_position count_lines(const char* begin, const char* end, ptrdiff_t offset)
{
	auto pos  = begin;
	auto last = pos + offset;
	auto prev = pos;

	assert(last <= end);
	last = std::min(last, end);

	size_t row = 1;

	while (pos++ < last) {
		if (*pos == '\n') {
			prev = pos;
			++row;
		}
	}

	size_t column = last - prev;

	return {row, column};
}

source_position count_lines(source_buffer& buffer, ptrdiff_t offset)
{
	return count_lines(std::begin(buffer), std::end(buffer), offset);
}
} // namespace script
} // namespace aw
