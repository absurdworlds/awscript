#include "linker.h"

#include "aw/script/driver/main.h"

#include "aw/script/codegen/backend.h"
#include "aw/script/diag/diagnostics_engine.h"
#include "aw/script/lexer/lexer.h"
#include "aw/script/lexer/source_manager.h"
#include "aw/script/parser/parser.h"
#include "aw/script/semantic/module_tree.h"
#include "aw/script/semantic/semantic_analyzer.h"
#include "aw/script/utility/ast_printer_default.h"

#include <aw/io/file.h>
#include <aw/types/array_view.h>
#include <aw/utility/string/join.h>
#include <aw/utility/on_scope_exit.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <chrono>

namespace aw::script::driver {
using clock = std::chrono::steady_clock;
struct timing {
	clock::time_point start;
	clock::time_point end;
};

std::map<std::string_view, timing> timings;

struct stopwatch {
	using clock = std::chrono::steady_clock;

	stopwatch(std::string_view name)
		: name(name)
		, start(clock::now())
	{
	}

	~stopwatch()
	{
		timings[name] = {
			.start = start,
			.end = clock::now(),
		};
	}

	std::string_view name;
	clock::time_point start;
};

void print_duration(std::string_view title, timing t)
{
	using duration = std::chrono::duration<double, std::micro>;
	std::cout << title << ": ";
	std::cout << duration(t.end - t.start).count() << "us\n";
}

auto parse_file(
	source_manager& srcman,
	diagnostics_engine& diag,
	std::string_view input)
	-> ast::module
{
	lexer lexer(srcman.get_buffer(srcman.add_file(input)));

	aw::script::parser parser({
		.lexer = lexer,
		.diag = diag
	});

	ast::module mod{
		.path = std::string(input),
	};

	while(true) {
		auto decl = parser.parse_top_level();
		if (!decl)
			break;
		mod.decls.push_back(std::move(*decl));
	}

	return mod;
}

auto parse_files(
	source_manager& srcman,
	diagnostics_engine& diag,
	array_view<std::string> file_list)
	-> std::vector<ast::module>
{
	stopwatch _("parse_files");

	std::vector<ast::module> modules;

	for (const auto& input : file_list)
	{
		modules.push_back(parse_file(srcman, diag, input));
	}

	return modules;
}


void dump_ast(array_view<ast::module> modules)
{
	ast_printer_default printer;
	for (const auto& mod : modules)
	{
		for (const auto& decl : mod.decls)
			printer.print_declaration(decl);
	}
}

// Temporary
auto create_module_tree(
	diagnostics_engine& diag,
	const std::vector<ast::module>& in_modules)
	-> module_tree
{
	stopwatch _("semantic_analyzer");

	module_tree mtree;

	semantic_analyzer analyzer(diag);

	for (auto& in_mod : in_modules)
	{
		auto input_path = std::filesystem::path(in_mod.path);

		auto mod = analyzer.lower(mtree, in_mod);
		mod.name = input_path.stem();
		mod.dir_path = input_path.parent_path();
		mtree.add_module(std::move(mod));
	}

	return mtree;
}

auto compile_modules(backend& backend, const module_tree& mtree, const options& options)
	-> std::vector<std::string>
{
	stopwatch _("backend");

	backend.set_target();
	backend.set_optimization_level(options.opt_level);

	std::vector<std::string> objects;
	for (const auto& [mod,_] : mtree.modules)
	{
		backend.create_module(mod);
		backend.optimize_module();
		if (options.dump_ir)
			backend.dump_ir();

		//TODO: write objects to a temporaty directory when mode == mode::link
		//if (options.mode == mode::make_obj)
		if (options.mode != mode::dry_run) {
			auto dir_path = std::filesystem::path(mod.dir_path);
			auto output = (dir_path/mod.name).replace_extension(".o").generic_string();
			backend.write_object_file(output);
			objects.push_back(output);
		}
	}

	return objects;
}

int run_compiler(const options& options)
{
	auto print_timings_guard = call_on_exit([&options] {
		if (!options.profile_compiler)
			return;
		for (const auto& [title, timing] : timings)
		{
			print_duration(title, timing);
		}
	});

	if (options.input_files.empty())
	{
		// TODO: use diagnostics_engine?
		std::cerr << "No input files given.\n";
		return EXIT_FAILURE;
	}

	source_manager srcman;

	diagnostics_engine diag(srcman);

	std::vector<ast::module> in_modules = parse_files(srcman, diag, options.input_files);

	if (options.dump_ast)
		dump_ast(in_modules);

	if (diag.has_error())
		return EXIT_FAILURE;

	in_modules = resolve_deps_timed(diag, std::move(in_modules));

	if (diag.has_error())
		return EXIT_FAILURE;

	module_tree mtree = create_module_tree(diag, in_modules);

	if (diag.has_error())
		return EXIT_FAILURE;

	auto backend = aw::script::backend::create("LLVM", diag);
	if (!backend)
		return EXIT_FAILURE;

	auto objects = compile_modules(*backend, mtree, options);

	std::string output_file = options.output_file;
	if (output_file.empty()) {
		output_file = std::filesystem::path(options.input_files[0])
			.replace_extension()
			.string();
	}

	if (options.mode == mode::link)
	{
		run_linker({
			.linker = default_linker(),
			.output_file = output_file,
			.objects = objects,
		});

		for (const auto& object : objects)
		{
			std::filesystem::remove(object);
		}
	}

	return EXIT_SUCCESS;
}

} // namespace aw::script::driver
