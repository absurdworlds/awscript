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
		modules.push_back({ std::move(mod) });
	}

	struct node {
		middle::module mod;
		std::map<std::string, node> submodules;
	};

	middle::module preamble;
	std::vector<node> modules;
};

} // namespace aw::script

#endif // aw_script_semantic_module_tree_h
