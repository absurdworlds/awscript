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
	minus,
	plus,
	multiply,
	divide,
	equal,
	not_equal,
	less,
	greater,
	assignment,
};
} // namespace aw::script::ast

#endif // aw_script_ast_operators_h
