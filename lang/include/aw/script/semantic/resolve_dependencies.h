#ifndef aw_script_semantic_resolve_dependencies_h
#define aw_script_semantic_resolve_dependencies_h

#include <aw/script/diag/diagnostics_engine.h>
#include <aw/script/ast/declaration.h>
#include <map>
#include <set>

namespace aw::script {

struct dependency_map {
	std::map<ast::unqual_id, dependency_map> modules;
	std::set<ast::unqual_id> ids;
};

auto resolve_dependencies(diagnostics_engine& diag, const ast::module& mod) -> dependency_map;

} // namespace aw::script

#endif // aw_script_semantic_resolve_dependencies_h
