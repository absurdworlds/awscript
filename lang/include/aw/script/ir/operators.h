#ifndef aw_script_ir_operators_h
#define aw_script_ir_operators_h

namespace aw::script::ir {

enum class unary_operator {
	minus,
	minus_fp,
	plus,
	negation,
	logical_not = negation
};

enum class binary_operator {
	minus,
	minus_fp,
	plus,
	plus_fp,
	multiply,
	multiply_fp,
	divide,
	divide_unsigned,
	divide_fp,
	modulo,
	modulo_unsigned,
	modulo_fp,
	bitwise_and,
	bitwise_xor,
	bitwise_or,
	shift_left,
	shift_right, // sign-extending shift
	shift_right_unsigned,
	less,
	less_unsigned,
	less_fp,
	less_equal,
	less_equal_unsigned,
	less_equal_fp,
	greater,
	greater_unsigned,
	greater_fp,
	greater_equal,
	greater_equal_unsigned,
	greater_equal_fp,
	equal,
	equal_fp,
	not_equal,
	not_equal_fp,
	logical_or,
	logical_and,
	assignment,
};

} // namespace aw::script::ir

#endif // aw_script_ir_operators_h
