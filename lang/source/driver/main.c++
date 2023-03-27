#include "aw/script/driver/main.h"

#include <aw/script/lexer/lexer.h>
#include <aw/script/lexer/source_manager.h>
#include <aw/script/parser/parser.h>
#include <aw/script/diag/diagnostics_engine.h>
#include <aw/script/semantic/semantic_analyzer.h>
#include <aw/script/codegen/backend.h>

#include <aw/utility/string/join.h>
#include <aw/io/file.h>

#include <filesystem>
#include <iostream>
#include <map>
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

	source_manager srcman;

	diagnostics_engine diag(srcman);

	std::map<std::string, ast::module> decl_source_map;

	for (const auto& input : options.input_files)
	{
		lexer lexer(srcman.get_buffer(srcman.add_file(input)));

		aw::script::parser parser({
			.lexer = lexer,
			.diag = diag
		});

		auto& decls = decl_source_map[input];

		while(true) {
			auto decl = parser.parse_top_level();
			if (!decl)
				break;
			callbacks->on_parse_declaration(*decl);
			decls.push_back(std::move(*decl));
		}
	}

	// Temporary
	semantic_analyzer analyzer;

	std::map<std::string, middle::module> mid_source_map;
	for (auto& [file,mod] : decl_source_map)
	{
		mid_source_map[file] = analyzer.lower(mod);
	}

	for (const auto& [_,decls] : decl_source_map)
		for (const auto& decl : decls)
			callbacks->process_declaration(decl);

	if (options.mode == mode::dry_run)
		return EXIT_SUCCESS;

	auto backend = aw::script::backend::create("LLVM", diag);
	if (!backend)
		return EXIT_FAILURE;

	backend->set_target();
	backend->set_optimization_level(options.opt_level);

	std::vector<std::string> objects;


	for (const auto& [input,decls] : mid_source_map)
	{
		auto input_path = std::filesystem::path(input);
		backend->create_module(input_path.stem().generic_string());
		for (const auto& decl : decls)
		{
			backend->handle_declaration(*decl);
		}
		backend->optimize_module();
		//TODO: write objects to a temporaty directory when mode == mode::link
		//if (options.mode == mode::make_obj)
		{
			auto output = input_path.replace_extension("o").generic_string();
			backend->write_object_file(output);
			objects.push_back(output);
			if (options.dump_ir)
				backend->dump_ir();
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
