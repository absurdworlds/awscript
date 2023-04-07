#include "context.h"

#include "aw/script/utility/wrap.h"

namespace aw::script {

auto context::create_type(const ast::type &type) -> ir::type*
{
	if (auto _ = get_if<ast::unknown_type>(&type)) {
		assert(!"Unknown type must be inferred");
		return nullptr;
	}

	if (auto ty = get_if<ast::regular_type>(&type)) {
		return find_type(ty->name);
	}

	// TODO
       assert(!"Not implemented")
	return nullptr;
}


} // namespace aw::script
