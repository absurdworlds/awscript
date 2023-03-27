#ifndef aw_script_ast_middle_declaration_h
#define aw_script_ast_middle_declaration_h

#include <aw/script/ast/middle/statement.h>

#include <aw/script/ast/mutability.h>
#include <aw/script/ast/type.h>

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace aw::script::middle {
struct variable {
	std::string name;
	ast::type* type = nullptr;
	ast::access access;
	std::unique_ptr<expression> value;
};

using parameter_list = std::vector<std::unique_ptr<variable>>;

struct function {
	std::string name;

	ast::type* return_type = nullptr;

	parameter_list args;

	statement body;
};

// TODO: proper modules
using module = std::vector<std::unique_ptr<struct declaration>>;

using declaration_variant = std::variant<
	variable,
	function
>;

struct declaration : public declaration_variant
{
	using declaration_variant::declaration_variant;
	using declaration_variant::operator=;

#if 0
	auto name() const -> std::string_view
	{
		auto visitor = [] (const auto& decl) -> std::string_view { return decl.name; };
		return std::visit(visitor, *this);
	}
#endif
};

} // namespace aw::script::middle

#endif // aw_script_ast_middle_declaration_h
