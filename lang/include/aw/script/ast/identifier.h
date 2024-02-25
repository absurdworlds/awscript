#ifndef aw_lang_ast_indentifier_h
#define aw_lang_ast_indentifier_h

#include <string>
#include <string_view>
#include <vector>

namespace aw::script::ast {

struct identifier {
	std::vector<std::string> path;
	std::string name;
};

inline bool operator==(const identifier& id, const std::string_view& sv)
{
	return id.path.empty() && id.name == sv;
}

inline auto to_string(const identifier& id) -> std::string
{
	// TODO: reserve
	std::string result;
	for (const auto& elem : id.path) {
		result += elem;
		result += "::";
	}
	result += id.name;
	return result;
}


} // namespace aw::script::ast

#endif // aw_lang_ast_indentifier_h
