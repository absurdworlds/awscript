#ifndef aw_script_context_h
#define aw_script_context_h

#include <aw/script/ast/type.h>
#include <aw/script/semantic/scope.h>

#include <cassert>

namespace aw::script {
struct context {
	scope default_scope;

	std::vector<std::unique_ptr<scope>> stack;

	// TODO: solve this mess
	std::vector<std::unique_ptr<ir::type>> types;


	void push_scope()
	{
		stack.push_back(std::make_unique<scope>(current_scope()));
	}
	void pop_scope()
	{
		stack.pop_back();
	}

	scope* current_scope()
	{
		return stack.empty() ? &default_scope : stack.back().get();
	}

	ir::type* find_type(std::string_view name)
	{
		return current_scope()->find_type(name);
	}

	ir::type* create_type(const ast::type& type);
	ir::type* add_type(ir::type&& type);
};
} // namespace aw::script

#endif // aw_script_context_h
