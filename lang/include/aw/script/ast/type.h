#ifndef aw_script_ast_type_h
#define aw_script_ast_type_h

#include "aw/script/ast/identifier.h"

#include <string>
#include <variant>
#include <vector>
#include <optional>

namespace aw::script::ast {

using type_name = identifier;

struct unknown_type {};

struct regular_type {
	type_name name;
};

struct pointer_suffix {};
struct reference_suffix {};
struct array_suffix {
	std::optional<size_t> size;
};

using type_suffix = std::variant<
	reference_suffix, // T&
	pointer_suffix,   // T*
	array_suffix      // T[] and T[N]
>;

struct composite_type {
	regular_type base;
	std::vector<type_suffix> suffix;
};

using type = std::variant<
	unknown_type,   // unknown type, to be inferred later
	regular_type,   // anything that doesn't fall into other categories
	composite_type  // T&, T*, T[] and T[N]
>;

} // namespace aw::script::ast

#endif // aw_script_ast_type_h
