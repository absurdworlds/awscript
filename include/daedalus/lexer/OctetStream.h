/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_OctetStream_
#define _daedalus_OctetStream_
#include <daedalus/lexer/Stream.h>

namespace daedalus {
/*! Interface for octet-based streams. */
class OctetStream : public Stream<char> {
public:
	typedef Stream<char>::primitive_type primitive_type;

	/*!
	 * Get currently extracted octet.
	 */
	virtual i32 getCurrent(primitive_type& out) = 0;

	/*!
	 * Extract next octet from stream.
	 */
	virtual i32 getNext(primitive_type& out) = 0;

	/*!
	 * Get current position in the stream.
	 */
	virtual size_t position() const = 0;
};
} // namespace daedalus
#endif//_daedalus_OctetStream_
