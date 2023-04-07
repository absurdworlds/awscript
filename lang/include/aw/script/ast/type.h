#ifndef aw_script_ast_type_h
#define aw_script_ast_type_h

#include <string>
#include <variant>
#include <vector>

namespace aw::script::ast {

using type_name = std::string;

struct unknown_type {};

struct regular_type {
	type_name name;
};

struct reference_type {
	type_name pointee;
};

struct pointer_type {
	type_name pointee;
};

using type = std::variant<
	unknown_type,   // unknown type, to be inferred later
	regular_type,   // anything that doesn't fall into other categories
	reference_type, // T&
	pointer_type    // T*
>;

} // namespace aw::script::ast

#endif // aw_script_ast_type_h
