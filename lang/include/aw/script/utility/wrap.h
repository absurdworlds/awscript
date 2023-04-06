#ifndef aw_utility_wrap_h
#define aw_utility_wrap_h

#include <memory>
#include <optional>

namespace aw::script {

template<typename Value, typename T = std::remove_reference_t<Value>>
auto wrap(Value&& init) -> std::unique_ptr<T>
{
	return std::make_unique<T>(std::forward<Value>(init));
}

template<typename T>
auto wrap(std::optional<T> init) -> std::unique_ptr<T>
{
	if (!init)
		return nullptr;
	return std::make_unique<T>(std::move(*init));
}

} // namespace aw::script

#endif // aw_utility_wrap_h
