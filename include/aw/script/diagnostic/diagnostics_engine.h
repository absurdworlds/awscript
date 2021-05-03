/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_diagnostic_engine_h
#define aw_script_diagnostic_engine_h
#include <iostream> //temporary
#include <aw/utility/string/compose.h>
#include <aw/script/diagnostic/diagnostic.h>
namespace aw {
namespace script {
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

	void report(Diagnostic diag)
	{
		array_view<std::string> args(diag.args);
		auto msg = aw::string::compose(getDiagMsg(diag.id), args);

		auto line   = diag.loc.pos;
		auto column = 0;
		if (_buffer)
		{
			auto pos = count_lines(*_buffer, line);
			line = pos.row;
			column = pos.column;
		}

		std::cerr << "error:" << line << ":" << column << ": " << msg << "\n";
	}

private:
	// TODO: replace with source_manager
	source_buffer* _buffer;
};
} // namespace script
} // namespace aw
#endif//aw_script_diagnostic_engine_h
