/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_codegen_backend_llvm_h
#define aw_script_codegen_backend_llvm_h

// TODO: put this into a separate static lib

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/expression.h>
#include <aw/script/ast/statement.h>

#include <aw/script/diag/diagnostics_engine.h>

#include <aw/script/codegen/backend.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/PassManager.h>

#include <llvm/Target/TargetMachine.h>
#include <map>

namespace aw::script {

class optimizer_llvm;

class backend_llvm : public backend {
public:
	explicit backend_llvm(diagnostics_engine& diag);

	~backend_llvm();

	bool set_target(string_view target_triple) override;
	void set_optimization_level(optimization_level level) override;

	bool create_module(string_view name) override;

	bool handle_declaration(const ast::declaration& decl) override
	{
		return gen(decl) != nullptr;
	}

	bool write_object_file(string_view out_path) override;

	void dump_ir() override;

	bool optimize_module() override;
	bool optimize_lto() override;

	auto gen(const ast::declaration& decl) -> llvm::Value*;
	auto gen(const ast::function& decl) -> llvm::Value*;
	auto gen(const ast::variable& var) -> llvm::Value*;

	auto gen(const std::unique_ptr<ast::statement>& stmt) -> llvm::Value*;
	auto gen(const ast::statement& stmt) -> llvm::Value*;
	auto gen(const ast::if_else_statement& stmt) -> llvm::Value*;
	auto gen(const ast::statement_block& stmt) -> llvm::Value*;
	auto gen(const ast::return_statement& stmt) -> llvm::Value*;
	auto gen(const ast::empty_statement& stmt) -> llvm::Value*;

	auto gen(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*;
	auto gen(const ast::expression& expr) -> llvm::Value*;
	auto gen_lvalue(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*;
	auto gen_lvalue(const ast::expression& expr) -> llvm::Value*;
	auto gen(const ast::numeric_literal& expr) -> llvm::Value*;
	auto gen(const ast::string_literal& expr) -> llvm::Value*;
	auto gen(const ast::value_expression& expr) -> llvm::Value*;
	auto gen(const ast::binary_expression& expr) -> llvm::Value*;
	auto gen(const ast::unary_expression& expr) -> llvm::Value*;
	auto gen(const ast::call_expression& expr) -> llvm::Value*;
	auto gen(const ast::if_expression& expr) -> llvm::Value*;

	template<typename T>
	auto gen(const T&) -> llvm::Value*
	{
		assert(false);
		return nullptr;
	}


	auto gen_if_condition(const std::unique_ptr<ast::expression>& expr) -> llvm::Value*;

	struct arg_info;

	auto create_function(const ast::function& decl, const std::vector<arg_info>& args) -> llvm::Function*;

private:
	diagnostics_engine& diag;
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder{context};
	llvm::TargetMachine* target_machine = nullptr;

	std::unique_ptr<optimizer_llvm> optimizer;
	std::unique_ptr<llvm::Module> cur_module;

	std::map<std::string, llvm::Value*, std::less<>> symtab;
	std::map<std::string_view, llvm::Value*, std::less<>> strings;
};

} // namespace aw::script
#endif//aw_script_codegen_backend_llvm_h
