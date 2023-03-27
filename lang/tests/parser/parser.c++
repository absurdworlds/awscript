
#include <aw/test/test.h>

#include <aw/script/lexer/lexer.h>
#include <aw/script/lexer/source_manager.h>
#include <aw/script/parser/parser.h>
#include <aw/script/diag/diagnostics_engine.h>

TestFile("parser-test")

using namespace std::string_view_literals;

namespace aw::script {

Test(basic_function) {
	aw::script::source_manager srcman;

	auto id = srcman.add_buffer(R"(
	function test_func(var int x) : int;

	function add(var int x, var int y) : int
	{
		-x;
		return x + y;
	}
)");

	aw::script::diagnostics_engine diag(srcman);

	aw::script::lexer lexer(srcman.get_buffer(id));

	aw::script::parser parser({
		.lexer = lexer,
		.diag = diag
	});

	std::optional<ast::declaration> decl;

	ast::declaration d1;
	ast::declaration d2;

	d1 = std::move(d2);

	do {
		decl = std::move(parser.parse_top_level());
	} while(decl);
}


} // namespace aw::script
