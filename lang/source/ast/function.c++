#include "aw/script/ast/declaration.h"

#include <aw/script/ast/statement.h>

#include <aw/script/symtab/scope.h>

namespace aw::script::ast {

function::function(std::string_view name)
	: name(name)
{

}

function::function(function&&) = default;

function::~function() = default;

function& function::operator=(function&&) = default;
} // namespace aw::script::ast
