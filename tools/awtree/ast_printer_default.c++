#include "ast_printer_default.h"

// TODO: for now it prints into iostream
// abstract it out later
#include <iostream>

namespace aw::script {
namespace {
struct visitor {
	void operator()()
	{

	}
};
} // namespace

ast_printer_default::ast_printer_default()
{

}

void ast_printer_default::print_declaration(const ast::declaration& decl)
{
	switch (decl.kind())
	{
	case ast::decl_kind::type:
	case ast::decl_kind::alias_type:
	case ast::decl_kind::class_type:
		break;
	case ast::decl_kind::function:
		print(decl.as<ast::function>());
		break;
	case ast::decl_kind::variable:
		break;
	}
}

void ast_printer_default::start(string_view name)
{
	print_indent();
	std::cout << '(' << name;
	++depth;
}

void ast_printer_default::end()
{
	--depth;
	print_indent();
	std::cout << ')';
}

void ast_printer_default::print_indent()
{
	if (depth > 0)
		std::cout << std::string(depth*2, ' ');
}

void ast_printer_default::print(const ast::function& decl)
{
	start(decl.name());
	for (const auto& arg : decl.args)
	{
		print(*arg);
	}

	print(*decl.body);
	end();
}

void ast_printer_default::print(const ast::variable& var)
{

}

void ast_printer_default::print(const ast::statement& stmt)
{

}

} // namespace aw::script
