#ifndef aw_script_ast_mutability_h
#define aw_script_ast_mutability_h

namespace aw::script::ast {

enum class access {
	variable, // a.k.a mutable
	constant
};

} // namespace aw::script::ast

#endif // aw_script_ast_mutability_h
