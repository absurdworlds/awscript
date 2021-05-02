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
// TODO:
// 1. Start building diagnostic
// 2. Add details
// 3. Report
/*!
 * This class is used to output diagnostic_engine messages.
 */
class DiagnosticsEngine {
public:
	DiagnosticsEngine()
	{
	}

	void report(Diagnostic diag)
	{
		auto msg = aw::string::compose(
		             getDiagMsg(diag.id), diag.args);
		std::cerr << msg << "\n";
	}
};
} // namespace script
} // namespace aw
#endif//aw_script_diagnostic_engine_h
