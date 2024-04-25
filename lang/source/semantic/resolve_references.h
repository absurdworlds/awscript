#ifndef aw_script_resolve_references_h
#define aw_script_resolve_references_h

#include "context.h"

#include "aw/script/diag/diagnostics_engine.h"
#include "aw/script/semantic/module_tree.h"

namespace aw::script {
namespace middle {
struct module;
} // namespace middle;

void resolve_references(const module_tree& mtree, context& ctx, diagnostics_engine& diag, middle::module& mod);

} // namespace aw::script

#endif // aw_script_resolve_references_h
