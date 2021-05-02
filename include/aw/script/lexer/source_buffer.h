/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_source_buffer_h
#define aw_script_source_buffer_h
#include <aw/script/common/types.h>
namespace aw {
namespace script {
/*!
 * Wraps access to source files
 */
class SourceBuffer {
public:
	virtual ~SourceBuffer() = default;
	// TODO: ptrwrapper
	char const* begin();
	char const* end();

	size_t getLength();
};
} // namespace script
} // namespace aw
#endif//aw_script_source_buffer_h
