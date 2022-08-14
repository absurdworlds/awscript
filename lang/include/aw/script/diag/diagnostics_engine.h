/*
 * Copyright (C) 2015-2022 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_diagnostic_engine_h
#define aw_script_diagnostic_engine_h
#include <aw/utility/string/compose.h>

#include <aw/script/diag/diagnostic.h>
namespace aw::script {
class source_buffer;
// TODO:
// 1. Start building diagnostic
// 2. Add details
// 3. Report
/*!
 * This class is used to output diagnostic_engine messages.
 */
class diagnostics_engine {
public:
	diagnostics_engine()
	{
	}

	diagnostics_engine(source_buffer& buffer)
		: _buffer(&buffer)
	{
	}

	// TODO: customizable output
	void report(diagnostic diag);

private:
	// TODO: replace with source_manager
	source_buffer* _buffer;
};
} // namespace aw::script
#endif//aw_script_diagnostic_engine_h
