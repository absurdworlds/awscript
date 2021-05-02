/*
 * Copyright (C) 2021   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_types_h
#define aw_script_types_h

#include <memory>
#include <aw/types/types.h>

namespace aw::script {

template<typename T>
using uptr = std::unique_ptr<T>;

} // namespace aw::script
#endif//aw_script_types_h
