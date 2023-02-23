#include <aw/script/symtab/symbol_table.h>

#include <aw/script/symtab/scope.h>

#include <aw/script/ast/decl/type.h>

namespace aw::script {
symbol_table::symbol_table()
{
	top_scope = create_scope();
	create_builtin_types();
}


void symbol_table::create_builtin_types()
{
	// TODO: move this out of here
	top_scope->add_symbol("void", create_type("void"));

	// TODO: aliases
	top_scope->add_symbol("int", create_type("int"));
	top_scope->add_symbol("i32", create_type("i32"));
	top_scope->add_symbol("i64", create_type("i64"));

	//top_scope->add_symbol("uint", create_type("uint"));
	//top_scope->add_symbol("u32", create_type("u32"));
	//top_scope->add_symbol("u64", create_type("u64"));

	top_scope->add_symbol("float", create_type("float"));
	top_scope->add_symbol("f32", create_type("f32"));
	top_scope->add_symbol("f64", create_type("f64"));

	// TODO: remove when FFI modules are implemented
	top_scope->add_symbol("cstring", create_type("cstring"));
}

ast::type* symbol_table::create_type(std::string_view name)
{
	types.push_back(std::make_unique<ast::type>(name));
	return types.back().get();
}

std::unique_ptr<ast::scope> symbol_table::create_scope()
{
	auto* cur_scope = current_scope();

	auto scope = std::make_unique<ast::scope>(cur_scope);

	stack.push_back(scope.get());

	return scope;
}

void symbol_table::pop_scope()
{
	stack.pop_back();
}

ast::declaration* symbol_table::lookup(std::string_view name)
{
	return current_scope()->find_symbol(name);
}

ast::type* symbol_table::lookup_type(std::string_view name)
{
	auto* p = lookup(name);
	if (!p || p->kind() != ast::decl_kind::type)
		return nullptr;
	return &p->as<ast::type>();
}

ast::scope* symbol_table::current_scope()
{
	return stack.empty() ? top_scope.get() : stack.back();
}

void symbol_table::add_unresolved(std::string_view type_name, ast::type** type)
{
	unresolved_types.push_back({
		.scope = current_scope(),
		.type = type,
		.name = type_name
	});
}

void symbol_table::resolve()
{
	for (const auto& ref : unresolved_types) {
		auto decl = ref.scope->find_symbol(ref.name);
		if (decl && decl->kind() == ast::decl_kind::type)
			*ref.type = static_cast<ast::type*>(decl);
	}
}

} // namespace aw::script
