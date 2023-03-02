/*
 * Copyright (C) 2016-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_utility_location_h
#define aw_script_utility_location_h
#include <aw/types/types.h>

#include <cassert>
#include <limits>
#include <string>
namespace aw::script {
struct location {
	location() = default;
	explicit location(size_t offset, u32 file_id = 0)
		: file_id(file_id)
		, pos(offset)
	{
		assert(offset < std::numeric_limits<u32>::max());
	}
	u32 file_id = 0;
	u32 pos     = 0;
};
} // namespace aw::script
#endif//aw_script_utility_location_h
