#ifndef aw_utility_variant_wrapper_h
#define aw_utility_variant_wrapper_h

namespace aw {

// I didn't know what to call this class,
// but it is needed due to clang's shenanigans
template<typename T>
struct hard_alias : T {
	hard_alias() = default;

	~hard_alias() = default;

	hard_alias(const hard_alias&) = delete;
	hard_alias(hard_alias&&) noexcept = default;

	hard_alias& operator=(const hard_alias&) = default;
	hard_alias& operator=(hard_alias&&) noexcept = default;
};

} // namespace aw

#endif // aw_utility_variant_wrapper_h
