
#include "llvm/backend_llvm.h"

#include <map>

namespace aw::script {

struct backend_factory
{
	virtual ~backend_factory() = default;
	virtual std::unique_ptr<backend> create(diagnostics_engine& diag) const = 0;
};

template<typename T>
struct generic_factory :  backend_factory {
	std::unique_ptr<backend> create(diagnostics_engine& diag) const override
	{
		return std::make_unique<T>(diag);
	}
};

generic_factory<backend_llvm> llvm_factory;

static std::map<std::string, backend_factory*, std::less<>> factory_registry = {
	{ "LLVM", &llvm_factory }
};

std::unique_ptr<backend> backend::create(std::string_view name, diagnostics_engine& diag)
{
	auto it = factory_registry.find(name);
	if (it == factory_registry.end())
		return nullptr;

	return it->second->create(diag);
}

} // namespace aw::script
