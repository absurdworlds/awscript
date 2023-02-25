#include "aw/script/ast/statement.h"
#include "aw/script/ast/declaration.h"

#include "aw/script/ast/middle/statement.h"
#include "aw/script/ast/middle/declaration.h"

#include "aw/script/utility/wrap.h"

namespace aw::script::ast {

// These are defined here to avoid "incomplete type" errors due to
// the circular dependency between decl and stmt
decl_statement::decl_statement(declaration&& decl)
	: decl(wrap(std::move(decl)))
{

}

decl_statement::decl_statement(std::unique_ptr<declaration> decl)
	: decl(std::move(decl))
{
}

decl_statement::~decl_statement() = default;
decl_statement::decl_statement(decl_statement&& other) noexcept = default;
decl_statement& decl_statement::operator=(decl_statement&& other) noexcept = default;

} // namespace aw::script::ast

namespace aw::script::middle {

// These are defined here to avoid "incomplete type" errors due to
// the circular dependency between decl and stmt
decl_statement::decl_statement(declaration&& decl)
	: decl(wrap(std::move(decl)))
{

}

decl_statement::decl_statement(std::unique_ptr<declaration> decl)
	: decl(std::move(decl))
{
}

decl_statement::~decl_statement() = default;
decl_statement::decl_statement(decl_statement&& other) noexcept = default;
decl_statement& decl_statement::operator=(decl_statement&& other) noexcept = default;

} // namespace aw::script::ast
