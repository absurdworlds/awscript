#include <aw/script/lexer/lexer.h>
#include <aw/script/parser/parser.h>
#include <aw/script/symtab/symbol_table.h>
#include <aw/script/symtab/scope.h>
#include <aw/script/diag/diagnostics_engine.h>
#include <aw/script/codegen/backend.h>

#include <aw/io/file.h>

#include <iostream>
#include <filesystem>

int main(int argc, char** argv)
{
	using namespace aw::script;
	if (argv[1] == nullptr)
		return 1;

	const std::filesystem::path input(argv[1]);
	const auto output = [input, arg = argv[2]] () -> std::filesystem::path {
		if (arg)
			return arg;
		auto output = input;
		return output.replace_extension("o");
	}();

	std::cout << input << " -> " << output << std::endl;

	aw::io::read_file<aw::io::file> file(input);

	source_buffer source(file);

	lexer lexer(&source);

	symbol_table symtab;
	diagnostics_engine diag(source);

	aw::script::parser parser({
		.lexer = lexer,
		.symtab = symtab,
		.diag = diag
	});


	std::vector<std::unique_ptr<ast::declaration>> decls;

	while(true) {
		auto decl = parser.parse_top_level();
		if (!decl)
			break;
		decls.push_back(std::move(decl));
	}

	symtab.resolve();

	auto backend = aw::script::backend::create("LLVM", diag);
	if (!backend)
		return EXIT_FAILURE;

	backend->set_target();

	for (const auto& decl : decls)
		backend->handle_declaration(*decl);

	backend->write_object_file(output.generic_string());
}
