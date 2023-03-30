#ifndef aw_script_ir_operators_h
#define aw_script_ir_operators_h

namespace aw::script::ir {

enum class unary_operator {
	minus,
	plus,
	negation,
};

enum class binary_operator {
	minus,
	plus,
	multiply,
	divide,
	less,
	less_unsigned,
	less_fp,
	greater,
	greater_unsigned,
	greater_fp,
	equal,
	not_equal,
	assignment,
};

} // namespace aw::script::ir

#endif // aw_script_ir_operators_h
