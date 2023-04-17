#ifndef aw_script_utility_type_name_h
#define aw_script_utility_type_name_h

#include <string>
#include <cassert>
#include <aw/script/ast/type.h>

namespace aw::script {
inline auto type_name(const ast::array_type& type) -> std::string
{
	auto arr_size = [] (std::optional<size_t> opt) {
		std::string result = "[";
		if (opt)
			result += std::to_string(*opt);
		result += "]";
		return result;
	};
	return type.elem + arr_size(type.size);
}

inline auto type_name(const ast::type& type) -> std::string
{
	if (auto _ = get_if<ast::unknown_type>(&type)) {
		return "<inferred type>";
	} else if (auto ty = get_if<ast::regular_type>(&type)) {
		return ty->name;
	} else if (auto ty = get_if<ast::pointer_type>(&type)) {
		return ty->pointee + "*";
	} else if (auto ty = get_if<ast::reference_type>(&type)) {
		return ty->pointee + "&";
	} else if (auto ty = get_if<ast::array_type>(&type)) {
		return type_name(*ty);
	} else {
		assert(!"Forgot to update the ast printer?");
		return "<unresolved type>";
	}
}

} // namespace aw::script

#endif // aw_script_utility_type_name_h
