#include "context.h"

#include "aw/script/utility/wrap.h"
#include "aw/script/utility/type_name.h"

namespace aw::script {

// TODO: use composite keys instead of strings?
// TODO: mangle name?
auto context::create_type(const ast::type &type) -> ir::type*
{
	if (auto _ = get_if<ast::unknown_type>(&type)) {
		assert(!"Unknown type must be inferred");
		return nullptr;
	}

	if (auto* ty = get_if<ast::regular_type>(&type)) {
		auto existing_ty = find_type(type_name(*ty));
		if (existing_ty)
			return existing_ty;

		return add_type(ir::type{ type_name(*ty) });
	}

	if (auto* comp = get_if<ast::composite_type>(&type)) {
		auto name = type_name(*comp);

		if (auto existing_ty = find_type(name))
			return existing_ty;

		auto& suffix = comp->suffix.back();
		auto copy = *comp;
		copy.suffix.pop_back();

		auto base = find_type(type_name(copy));

		if (auto* arr = get_if<ast::array_suffix>(&suffix)) {
			return add_type(ir::type{
				.name = name,
				.kind = ir::array_type{
					.base_type = base,
					.size = arr->size,
				}
			});
		}

		if (auto ptr = get_if<ast::pointer_suffix>(&suffix)) {
			return add_type(ir::type{
				.name = name,
				.kind = ir::pointer_type{
					.base_type = base,
					.is_mutable = false, // for now
				}
			});
		}
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
