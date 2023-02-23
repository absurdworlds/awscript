#include "aw/script/driver/main.h"

#include <aw/script/lexer/lexer.h>
#include <aw/script/parser/parser.h>
#include <aw/script/symtab/symbol_table.h>
#include <aw/script/symtab/scope.h>
#include <aw/script/diag/diagnostics_engine.h>
#include <aw/script/codegen/backend.h>

#include <aw/utility/string/join.h>
#include <aw/io/file.h>

#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace aw::script::driver {

static callbacks null_callbacks;

int run_compiler(const options& options, callbacks* callbacks)
{
	// If callbacks are not specified, use empty callbacks
	// so that we don't have to make a bunch of null checks
	if (!callbacks)
		callbacks = &null_callbacks;


	if (options.input_files.empty())
	{
		// TODO: error
		return EXIT_FAILURE;
	}

	symbol_table symtab;

	diagnostics_engine diag;

	// TODO: merge with the symbol_table
	using decl_list = std::vector<std::unique_ptr<ast::declaration>>;
	// TODO: base it on module instead
	std::map<std::string, decl_list> decl_source_map;

	// TODO: replace with source_manager
	std::vector<source_buffer> sources;

	for (const auto& input : options.input_files)
	{
		aw::io::read_file<aw::io::file> file(input);
		sources.emplace_back(file);
		auto& source = sources.back();
		// TODO: lift it out
		diagnostics_engine diag(source);

		lexer lexer(&source);

		aw::script::parser parser({
			.lexer = lexer,
			.symtab = symtab,
			.diag = diag
		});

		auto& decls = decl_source_map[input];

		while(true) {
			auto decl = parser.parse_top_level();
			if (!decl)
				break;
			callbacks->on_parse_declaration(*decl);
			decls.push_back(std::move(decl));
		}
	}

	symtab.resolve();

	for (const auto& [_,decls] : decl_source_map)
		for (const auto& decl : decls)
			callbacks->process_declaration(*decl);

	if (options.mode == mode::dry_run)
		return EXIT_SUCCESS;

	auto backend = aw::script::backend::create("LLVM", diag);
	if (!backend)
		return EXIT_FAILURE;

	backend->set_target();

	std::vector<std::string> objects;

	for (const auto& [input,decls] : decl_source_map)
	{
		for (const auto& decl : decls)
		{
			backend->handle_declaration(*decl);
		}
		//TODO: write objects to a temporaty directory when mode == mode::link
		//if (options.mode == mode::make_obj)
		{
			auto output = std::filesystem::path(input).replace_extension("o").generic_string();
			backend->write_object_file(output);
			objects.push_back(output);
		}
	}

	if (options.mode == mode::link)
	{
		// This is a big TODO, I need to figure out how to find the linker,
		// and how to know what to pass to it
	#if 0
		std::string linker_invocation = "ld -m elf_x86_64 -pie -dynamic-linker /usr/lib/ld-linux-x86-64.so.2 -lc";
		linker_invocation += "-o ";
		linker_invocation += output.stem();
		linker_invocation += " /usr/lib/Scrt1.o ";
		linker_invocation += output;
	#endif

		std::string linker_invocation = "g++ -o";
		linker_invocation += options.output_file;
		linker_invocation += ' ';
		linker_invocation += aw::string::join(objects, " ");

		// TODO: at the very least, replace system() with a proper invocation
		system(linker_invocation.c_str());

		for (const auto& object : objects)
		{
			std::filesystem::remove(object);
		}
	}

	return EXIT_SUCCESS;
}

} // namespace aw::script::driver
