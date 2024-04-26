#ifndef aw_script_utility_type_name_h
#define aw_script_utility_type_name_h

#include "aw/script/ast/type.h"

#include <cassert>
#include <string>

namespace aw::script {
inline auto type_suffix(const ast::pointer_suffix& /*suffix*/) -> std::string
{
	return "*";
}

inline auto type_suffix(const ast::reference_suffix& /*suffix*/) -> std::string
{
	return "&";
}

inline auto type_suffix(const ast::array_suffix& suffix) -> std::string
{
	std::string result = "[";
	if (suffix.size)
		result += std::to_string(*suffix.size);
	result += "]";
	return result;
}

inline auto type_suffix(const ast::type_suffix& suffix) -> std::string
{
	const auto visitor = [] (auto& v) -> std::string { return type_suffix(v); };
	return std::visit(visitor, suffix);
}


inline auto type_name(const ast::identifier& id) -> std::string
{
	return to_string(id);
}

inline auto type_name(const ast::unknown_type& /*type*/) -> std::string
{
	return "<inferred type>";
}

inline auto type_name(const ast::regular_type& type) -> std::string
{
	return type_name(type.name);
}

inline auto type_name(const ast::composite_type& type) -> std::string
{
	std::string name = type_name(type.base);
	for (const auto& suffix : type.suffix)
		name += type_suffix(suffix);
	return name;
}

inline auto type_name(const ast::type& type) -> std::string
{
	const auto visitor = [] (auto& v) -> std::string { return type_name(v); };
	return std::visit(visitor, type);
}

} // namespace aw::script

#endif // aw_script_utility_type_name_h
