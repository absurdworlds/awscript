/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_parser_foreign_module_parser_h
#define aw_parser_foreign_module_parser_h

#include <aw/script/lexer/lexer.h>

#include <aw/script/symtab/symbol_table.h>

#include <aw/script/diag/diagnostics_engine.h>

namespace aw::script {
class foreign_module_parser
{
public:
	struct dependencies {
		script::lexer& lexer;
		symbol_table& symtab;
		diagnostics_engine& diag;
	};

	foreign_module_parser();

	auto parse_top_level() -> std::unique_ptr<ast::declaration>;
};

} // namespace aw::script

#endif // aw_parser_foreign_module_parser_h
