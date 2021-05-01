/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_diagnostic_engine_h
#define hrscript_diagnostic_engine_h
#include <iostream> //temporary
#include <aw/string/compose.h>
#include <hrscript/diagnostic/Diagnostic.h>
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
} // namespace hrscript
} // namespace aw
#endif//hrscript_diagnostic_engine_h
