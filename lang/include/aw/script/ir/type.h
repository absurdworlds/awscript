#ifndef aw_script_ir_type_h
#define aw_script_ir_type_h

#include <aw/script/utility/map.h>

#include <string>
#include <variant>
#include <vector>
#include <optional>

namespace aw::script::middle {
struct struct_decl;
struct expression;
} // namespace aw::script::middle

namespace aw::script::ir {

struct type;

struct unknown_type {};

enum class simple_type {
	nothing,
	string_literal,
	numeric_literal,
	float_literal,
	boolean,
	byte,
	character,
};

struct integer_type {
	int size = 0;
	bool is_signed = false;
};

struct fp_type {
	int size = 0;
};

struct pointer_type {
	type* base_type = nullptr;
	bool is_mutable = false;
};

struct array_type {
	type* base_type = nullptr;
	std::optional<size_t> size;
};

struct reference_type {
	type* base_type = nullptr;
	bool is_mutable = false;
};

struct alias_type {
	type* base_type = nullptr;
	bool strict = false;
};

struct variant_type {
	std::vector<type*> alternatives;
};

struct tuple_type {
	std::vector<type*> members;
};

struct struct_type {
	// TODO: rethink this
	middle::struct_decl* decl;

	struct field {
		std::string name;
		ir::type* type = nullptr;
		middle::expression const* init = nullptr;
	};

	map<std::string, field> fields;
};

using type_kind = std::variant<
	unknown_type,
	simple_type,
	integer_type,
	fp_type,
	array_type,
	pointer_type,
	reference_type,
	alias_type,
	variant_type,
	tuple_type,
	struct_type
>;

struct type {
	std::string name;
	type_kind kind;
};

} // namespace aw::script::ir

#endif // aw_script_ir_type_h
