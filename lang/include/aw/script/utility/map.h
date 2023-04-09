#ifndef aw_script_utility_map_h
#define aw_script_utility_map_h

#include <unordered_map>

namespace aw::script {

template<typename Key, typename Value>
struct map : std::unordered_map<Key,Value> {
	using base = std::unordered_map<Key,Value>;

	using base::base;
	using base::operator=;

	template<typename T>
	auto find(T&& key) -> Value*
	{
		auto it = base::find(key);
		if (it == base::end())
			return nullptr;
		return &it->second;
	}
};

} // namespace aw::script

#endif // aw_script_utility_map_h
