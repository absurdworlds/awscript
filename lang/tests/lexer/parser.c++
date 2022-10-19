
#include <aw/test/test.h>

#include <aw/script/lexer/lexer.h>
#include <aw/script/parser/parser.h>
#include <aw/script/symtab/symbol_table.h>
#include <aw/script/symtab/scope.h>
#include <aw/script/diag/diagnostics_engine.h>

TestFile("parser-test")

using namespace std::string_view_literals;

namespace aw::script {

Test(basic_function) {
	auto source = source_buffer(R"(
	func test_func() : int;
)");

	aw::script::lexer lexer(&source);

	aw::script::symbol_table symtab;
	aw::script::diagnostics_engine diag;

	aw::script::parser parser({
		.lexer = lexer,
		.symtab = symtab,
		.diag = diag
	});

	std::unique_ptr<ast::declaration> decl;

	do {
		decl = parser.parse_top_level();
	} while(decl);
}


} // namespace aw::script
