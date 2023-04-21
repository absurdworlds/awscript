#ifndef aw_utility_variant_wrapper_h
#define aw_utility_variant_wrapper_h

namespace aw {

// I didn't know what to call this class,
// but it is needed due to clang's shenanigans

template<typename T>
struct noncopyable : T {
	noncopyable() = default;

	~noncopyable() = default;

	noncopyable(const noncopyable&) = delete;
	noncopyable(noncopyable&&) noexcept = default;

	noncopyable& operator=(const noncopyable&) = delete;
	noncopyable& operator=(noncopyable&&) noexcept = default;
};

template<typename T>
using hard_alias = noncopyable<T>;

} // namespace aw

#endif // aw_utility_variant_wrapper_h
