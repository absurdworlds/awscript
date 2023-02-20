#pragma once

#include <aw/script/ast/declaration.h>

namespace aw::script {

class ast_printer {
public:
	virtual void print_declaration(const ast::declaration& decl) = 0;
};

} // namespace aw::script
