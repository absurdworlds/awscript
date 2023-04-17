#ifndef aw_script_type_inferrence_h
#define aw_script_type_inferrence_h

#include "context.h"

#include <aw/script/ast/middle/declaration.h>

namespace aw::script {
class diagnostics_engine;

struct type_hint {
	type_hint(std::nullptr_t)
		: is_ambiguous(true)
	{}
	type_hint(ir::type* type)
		: type(type), is_ambiguous(!type)
	{}
	type_hint(ir::type* type, bool is_ambiguous)
		: type(type), is_ambiguous(is_ambiguous)
	{}

	operator ir::type*() const { return type; }

	ir::type* type = nullptr;
	bool is_ambiguous = false;
};

auto infer_type(context& ctx, diagnostics_engine& diag, middle::expression& expr) -> type_hint;
auto propagate_type(context& ctx, diagnostics_engine& diag, ir::type* type, middle::expression& expr) -> ir::type*;

} // namespace aw::script

#endif // aw_script_type_inferrence_h
