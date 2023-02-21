#include "ast_printer_awdoc.h"
#include "ast_printer_default.h"

#include <aw/script/lexer/lexer.h>
#include <aw/script/parser/parser.h>
#include <aw/script/symtab/symbol_table.h>
#include <aw/script/symtab/scope.h>
#include <aw/script/diag/diagnostics_engine.h>

#include <aw/io/file.h>

#include <iostream>

int main(int argc, char** argv)
{
	using namespace aw::script;

	if (argv[1] == 0)
		return 1;

	std::cout << argv[1] << std::endl;

	aw::io::read_file<aw::io::file> file(argv[1]);

	source_buffer source(file);

	lexer lexer(&source);

	symbol_table symtab;
	diagnostics_engine diag(source);

	aw::script::parser parser({
		.lexer = lexer,
		.symtab = symtab,
		.diag = diag
	});

	ast_printer_default printer;

	std::vector<std::unique_ptr<ast::declaration>> decls;

	while(true) {
		auto decl = parser.parse_top_level();
		if (!decl)
			break;
		decls.push_back(std::move(decl));
	}

	symtab.resolve();

	for (const auto& decl : decls)
		printer.print_declaration(*decl);
#if 0
	backend_llvm backend(diag);
	for (const auto& decl : decls)
		backend.gen(decl);
	backend.create_object();
#endif
}
