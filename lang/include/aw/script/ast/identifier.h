#ifndef aw_lang_ast_indentifier_h
#define aw_lang_ast_indentifier_h

#include <vector>
#include <string_view>

namespace aw::script::ast {

struct identifier {
	std::vector<std::string_view> path;
	std::string_view name;
};

} // namespace aw::script

#endif // aw_lang_ast_indentifier_h
