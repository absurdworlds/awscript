#ifndef aw_script_ast_middle_declaration_h
#define aw_script_ast_middle_declaration_h

#include <aw/script/ast/middle/statement.h>

#include <aw/script/ast/mutability.h>
#include <aw/script/ir/type.h>

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace aw::script::middle {
struct no_initializer {};

struct struct_initializer {
	struct field {
		std::string name;
		expression value;
	};
	using field_vec = hard_alias<std::vector<field>>;
	field_vec fields;
};

struct expr_initializer {
	expression value;
};

using initializer_variant = std::variant<
	no_initializer,
	struct_initializer,
	expr_initializer
>;

struct initializer : initializer_variant {
	using initializer_variant::initializer_variant;
	using initializer_variant::operator=;
};

struct variable {
	std::string name;
	ir::type* type = nullptr;
	ast::access access;
	std::unique_ptr<initializer> init;
};

using parameter_list = std::vector<std::unique_ptr<variable>>;

struct function {
	std::string name;

	ir::type* return_type = nullptr;

	parameter_list args;
	bool is_variadic = false;

	std::unique_ptr<statement> body;
};

struct struct_decl {
	std::string name;

	std::vector<std::unique_ptr<variable>> members;
};

struct declaration;

// TODO: proper modules
struct module {
	std::string name;
	std::string dir_path;
	// Items declared in the module
	std::vector<std::unique_ptr<declaration>> decls;
	// Types instantiated in the module
	std::vector<std::unique_ptr<ir::type>> types;
};

using declaration_variant = std::variant<
	variable,
	function,
	struct_decl
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
