#include <aw/script/symtab/symbol_table.h>

#include <aw/script/symtab/scope.h>

namespace aw::script {
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

} // namespace aw::script
