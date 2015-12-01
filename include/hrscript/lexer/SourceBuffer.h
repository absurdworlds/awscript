/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef awscript_source_buffer_h_
#define awscript_source_buffer_h_
#include <aw/common/types.h>
namespace aw {
namespace script {
/*!
 * Wraps access to source files
 */
class SourceBuffer {
public:
	// TODO: ptrwrapper
	char const* begin();
	char const* end();

	size_t getLength();
};
} // namespace script
} // namespace aw
#endif//awscript_source_buffer_h_
