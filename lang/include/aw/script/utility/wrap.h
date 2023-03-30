#ifndef aw_utility_wrap_h
#define aw_utility_wrap_h

#include <memory>

namespace aw::script {

template<typename Value, typename T = std::remove_reference_t<Value>>
auto wrap(Value&& init) -> std::unique_ptr<T>
{
	return std::make_unique<T>(std::forward<Value>(init));
}

} // namespace aw::script

#endif // aw_utility_wrap_h
