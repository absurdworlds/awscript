/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_backend_llvm_h
#define hrscript_backend_llvm_h
#include <hrscript/backend/Backend.h>

namespace hrscript {
class BackendLLVM : public backend {
public:
	Backend();
	virtual ~Backend() = default;
private:
	std::unique_ptr<llvm::Module> module;
	std::map<std::string, std::unique_ptr<llvm::Value>> values;
	llvm::IRBuilder<> builder;
};
} // namespace hrscript
#endif//hrscript_backend_llvm_h
