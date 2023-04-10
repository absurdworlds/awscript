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
	assign_minus,
	assign_plus,
	assign_multiply,
	assign_divide,
	assign_modulo,
	assign_or,
	assign_xor,
	assign_and,
	assign_shift_left,
	assign_shift_right,
	// other operators
	access,
	range,
};
} // namespace aw::script::ast

#endif // aw_script_ast_operators_h
