#ifndef aw_script_ast_operators_h
#define aw_script_ast_operators_h

namespace aw::script::ast {
enum class unary_operator {
	minus,
	plus,
	logical_negation,
	binary_negation,
};

enum class binary_operator {
	// math operators
	minus,
	plus,
	multiply,
	divide,
	modulo,
	// bitwise operators
	bitwise_and,
	bitwise_xor,
	bitwise_or,
	shift_left,
	shift_right,
	// comparison operators
	equal,
	not_equal,
	less,
	less_equal,
	greater,
	greater_equal,
	compare,
	// logical operators
	logical_or,
	logical_and,
	// assignment operators
	assignment,
	// other operators
	access,
	range,
};
} // namespace aw::script::ast

#endif // aw_script_ast_operators_h
