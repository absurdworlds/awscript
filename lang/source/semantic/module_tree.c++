#include "aw/script/semantic/module_tree.h"

#include "context.h"

#include <aw/script/utility/wrap.h>


namespace aw::script {

auto create_type(std::string_view name) -> std::unique_ptr<ir::type>
{
	ir::type type;
	type.name = std::string(name);
	return wrap(std::move(type));
}

auto create_preamble() -> middle::module
{
	middle::module mod { .name = "$preamble" };

	auto& types = mod.types;

	auto create_int = [&types] (char kind, int size, std::string_view name = {}) {
		std::string type_name;
		if (name.empty()) {
			type_name += kind;
			type_name += std::to_string(size);
		} else {
			type_name = name;
		}

		types.push_back(wrap(ir::type{
			type_name,
			ir::integer_type{
				.size = size,
				.is_signed = (kind != 'u')
			}
		}));

		return types.back().get();
	};

	types.push_back(create_type("void"));
	types.push_back(wrap(ir::type{ "bool", ir::simple_type::boolean }));

	auto u8 = create_int('u', 8);
	create_int('u', 16);
	create_int('u', 32);
	create_int('u', 64);
	create_int('u', 64, "usize");
	create_int('u', 128);

	create_int('i', 8);
	create_int('i', 16);
	create_int('i', 32);
	create_int('i', 32, "int"); // TODO: alias for 32
	create_int('i', 64);
	create_int('i', 64, "isize");
	create_int('i', 128);

	types.push_back(wrap(ir::type{"float",  ir::fp_type{ 32 }})); // TODO: aliases
	types.push_back(wrap(ir::type{"double", ir::fp_type{ 64 }})); // TODO: aliases
	types.push_back(wrap(ir::type{"f32",    ir::fp_type{ 32 }}));
	types.push_back(wrap(ir::type{"f64",    ir::fp_type{ 64 }}));

	types.push_back(wrap(ir::type{"u8*",  ir::pointer_type{
		.base_type = u8,
		.is_mutable = false,
	}}));

	types.push_back(create_type("numeric_literal"));
	types.push_back(create_type("string_literal"));
	types.push_back(create_type("float_literal"));


	return mod;
}

module_tree::module_tree()
	: preamble(create_preamble())
{

}


} // namespace aw::script
