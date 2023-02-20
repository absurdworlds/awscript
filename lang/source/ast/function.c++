#include "aw/script/ast/decl/function.h"

#include <aw/script/symtab/scope.h>

namespace aw::script::ast {
function::function(std::string_view name)
    : declaration(decl_kind::function, name)
{}

function::~function() {}

void function::add_arg(std::unique_ptr<variable> arg)
{
	scope->add_symbol(arg->name(), arg.get());
	args.push_back(std::move(arg));
}
} // namespace aw::script::ast
