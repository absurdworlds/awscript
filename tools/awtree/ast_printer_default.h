#pragma once

#include "ast_printer.h"

#include <aw/script/ast/decl/function.h>

namespace aw::script {
class ast_printer_default : public ast_printer
{
public:
	ast_printer_default();

	void print_declaration(const ast::declaration& decl) override;

private:
	void print(const ast::function& decl);
	void print(const ast::variable& var);
	void print(const ast::statement& stmt);

	void print_indent();

	void start(string_view name);
	void end();

	int depth = 0;
};

} // namespace aw::script
