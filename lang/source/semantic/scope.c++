#include <aw/script/semantic/scope.h>

namespace aw::script {

auto scope::find_symbol(std::string_view name, scope_search_mode mode) -> declaration*
{
	auto* scope = this;

	auto it = scope->symbols.find(name);
	if (mode == scope_search_mode::including_parents)
	{
		while (it == scope->symbols.end() && scope->parent_scope)
		{
			scope = scope->parent_scope;
			it = scope->symbols.find(name);
		}
	}

	return it == scope->symbols.end() ? nullptr : &it->second;
}

} // namespace aw::script
