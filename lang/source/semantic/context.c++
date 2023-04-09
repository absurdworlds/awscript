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
		auto existing_ty = find_type(ty->name);
		if (existing_ty)
			return existing_ty;

		return add_type(ir::type{ ty->name });
	}

	if (auto ptr = get_if<ast::pointer_type>(&type)) {
		auto name = ptr->pointee + "*";

		// TODO: use composite keys instead of strings?
		if (auto existing_ty = find_type(name))
			return existing_ty;

		auto pointee = find_type(ptr->pointee);

		return add_type(ir::type{
			.name = name,
			.kind = ir::pointer_type{
				.base_type = pointee,
				.is_mutable = false, // for now
			}
		});
	}

	// TODO
	assert(!"Not implemented");
	return nullptr;
}

auto context::add_type(ir::type&& type) -> ir::type*
{
	auto existing_ty = find_type(type.name);
	if (existing_ty) {
		if (get_if<ir::unknown_type>(&existing_ty->kind))
			*existing_ty = std::move(type);
		return existing_ty;
	}

	auto new_type = wrap(std::move(type));
	auto type_ref = new_type.get();

	types.push_back(std::move(new_type));

	default_scope.add_type(type_ref->name, type_ref);

	return type_ref;
}


} // namespace aw::script
