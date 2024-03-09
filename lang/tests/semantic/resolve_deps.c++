
#include <aw/test/test.h>

#include <aw/script/ast/declaration.h>
#include <aw/script/semantic/resolve_dependencies.h>
#include <aw/script/lexer/source_manager.h>
#include <aw/script/utility/wrap.h>

TestFile("resolve-deps-test")

namespace aw::script {

Test(predefined_ast) {
	auto mod = ast::module{
		.name = "main"
	};

	mod.decls.emplace_back(ast::import_decl{
		.mod_id = { .name = "test1" },
		.decl_ids = { "func1", "func2" }
	});


	auto f_body = ast::statement_block{};
	f_body.emplace_back(ast::call_expression{
		.func = { .name = "func1" }
	});
	f_body.emplace_back(ast::call_expression{
		.func = { .path = {"test1"}, .name = "func2" }
	});
	f_body.emplace_back(ast::call_expression{
		.func = { .path = {"test2"}, .name = "func2" }
	});

	auto f = ast::function{
		.name = "f",
		.return_type = ast::regular_type{
			.name = {
				.path = { "aw", "math" },
				.name = "vec3"
			}
		},
		.body = wrap(ast::statement(std::move(f_body))),
	};

	f.params.emplace_back(ast::variable{
		.name = "a",
		.type = ast::regular_type{
			.name = {
				.path = { "aw", "math" },
				.name = "vec3"
			}
		}
	});

	mod.decls.push_back(std::move(f));


	source_manager srcman;
	diagnostics_engine diag(srcman);
	auto deps = resolve_dependencies(diag, mod);

	TestEqual(deps.modules.size(), 3);
	TestAssert(deps.ids.empty());
	TestAssert(deps.modules.contains("aw"));
	TestAssert(deps.modules.contains("test1"));
	TestAssert(deps.modules.contains("test2"));
	TestAssert(deps.modules["test1"].ids.contains("func1"));
	TestAssert(deps.modules["test1"].ids.contains("func2"));
	TestEqual(deps.modules["test1"].ids.size(), 2);
	TestAssert(deps.modules["test2"].ids.contains("func2"));
	TestEqual(deps.modules["test2"].ids.size(), 1);
}

} // namespace aw::script
