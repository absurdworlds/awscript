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

	if (auto ptr = get_if<ast::pointer_type>(&type)) {
		auto name = ptr->pointee + "*";

		// TODO: use composite keys instead of strings?
		if (auto ty = find_type(name))
			return ty;

		auto pointee = find_type(ptr->pointee);

		auto new_type = wrap(
			ir::type{
				.name = name,
				.kind = ir::pointer_type{
					.base_type = pointee,
					.is_mutable = false, // for now
				}
			}
		);

		auto result = new_type.get();

		types.push_back(std::move(new_type));

		default_scope.add_type(name, result);

		return result;
	}

	// TODO
       assert(!"Not implemented")
	return nullptr;
}


} // namespace aw::script
