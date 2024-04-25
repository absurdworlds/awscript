#ifndef aw_script_semantic_module_tree_h
#define aw_script_semantic_module_tree_h
#include "aw/script/ast/middle/module.h"

#include <map>
#include <string>

namespace aw::script {

class module_tree {
public:
	module_tree();

	void add_module(middle::module mod)
	{
		// TODO
		map.emplace(ast::identifier{ .name = mod.name }, modules.size());
		modules.push_back({ std::move(mod) });
	}

	auto find_module(const ast::identifier& id) const -> const middle::module*
	{
		auto it = map.find(id);
		if (it == map.end())
			return nullptr;
		return &modules[it->second];
	}

	middle::module preamble;
	std::map<ast::identifier, size_t> map;
	std::vector<middle::module> modules;
};

} // namespace aw::script

#endif // aw_script_semantic_module_tree_h
