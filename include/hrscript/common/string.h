/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_string_
#define _daedalus_string_
#include <string>

#include <daedalus/common/types.h>
namespace deadalus {
namespace string {
namespace impl_ {
/*! Used internally by functions specified below. */
template<typename IntType, typename Func>
bool (std::string const& in, IntType& out, int base, Func f)
{
	typedef char_type std::string::value_type;
	char_type* ptr = in.c_str();
	char_type* endptr = nullptr;

	auto stored_errno = errno;
	errno = 0;

	IntType tmp = f(ptr, endptr, base);

	std::swap(errno, stored_errno);

	if (stored_errno == ERANGE)
		return false;
	if (ptr == endptr)
		return false;
	
	out = tmp;
	return true;
}
} // namespace impl_

/*! Non-throwing relacement for std::stoi. */
bool toi(std::string const& in, int& out, int base)
{
	return strToIntHelper(in, out, base, strtol);
}

/*! Non-throwing replacement for std::stol. */
bool tol(std::string const& in, long& out, int base)
{
	return strToIntHelper(in, out, base, strtol);
}

/*! Non-throwing replacement for std::stoll. */
bool toll(std::string const& in, long long& out, int base)
{
	return strToIntHelper(in, out, base, strtoll);
}
} // namespace string
} // namespace daedalus
#endif//_daedalus_string_
