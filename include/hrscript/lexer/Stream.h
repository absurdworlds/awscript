/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_stream_
#define _hrscript_stream_
#include <hrscript/common/types.h>

namespace hrscript {
/*!
 * This class provides unified interface for sequential data access.
 */
template <typename T>
class Stream {
public:
	typedef T primitive_type;

	/*!
	 * Get currently extracted primitive.
	 */
	virtual i32 getCurrent(primitive_type& out) = 0;

	/*! Extract next primitive from stream. */
	virtual i32 getNext(primitive_type& out) = 0;

	/*!
	 * Get current position in the stream.
	 */
	virtual size_t position() const = 0;
};

} // namespace hrscript
#endif//_hrscript_stream_
