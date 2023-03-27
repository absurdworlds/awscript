#ifndef aw_script_semantic_analyzer_h
#define aw_script_semantic_analyzer_h

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/middle/declaration.h>

namespace aw::script {

class scope;
class context;

// TODO: split into "ast_lowering" and "type_check"
class semantic_analyzer {
public:
	semantic_analyzer();

	auto lower(const ast::module& decl) -> middle::module;

private:
	// TODO: should be stored in the builtin_module, not here
	std::vector<std::unique_ptr<ast::type>> builtin_types;

	auto visit(context& ctx, const ast::declaration& decl) -> middle::declaration;
	auto visit(context& ctx, const ast::parameter_list& var) -> middle::parameter_list;
	auto visit(context& ctx, const ast::function& decl) -> middle::function;
	auto visit(context& ctx, const ast::variable& var) -> middle::variable;

	auto visit_stmt(context& ctx, const ast::statement& in_stmt) -> middle::statement;
	auto visit_stmt(context& ctx, const ast::statement_block& in_stmt) -> middle::statement_block;
	auto visit_stmt(context& ctx, const ast::if_else_statement& in_stmt) -> middle::if_else_statement;
	auto visit_stmt(context& ctx, const ast::return_statement& in_stmt) -> middle::return_statement;
	auto visit_stmt(context& ctx, const ast::empty_statement& in_stmt) -> middle::empty_statement;
	auto visit_stmt(context& ctx, const ast::expression& in_stmt) -> middle::expression;

	auto visit_expr(context& ctx, const ast::expression& in_expr) -> middle::expression;
	auto visit_expr(context& ctx, const ast::unary_expression& in_expr) -> middle::unary_expression;
	auto visit_expr(context& ctx, const ast::binary_expression& in_expr) -> middle::binary_expression;
	auto visit_expr(context& ctx, const ast::call_expression& in_expr) -> middle::call_expression;
	auto visit_expr(context& ctx, const ast::if_expression& in_expr) -> middle::if_expression;
	auto visit_expr(context& ctx, const ast::value_expression& in_expr) -> middle::value_expression;
	auto visit_expr(context& ctx, const ast::numeric_literal& in_expr) -> middle::numeric_literal;
	auto visit_expr(context& ctx, const ast::string_literal& in_expr) -> middle::string_literal;
};


} // namespace aw::script

#endif // aw_script_semantic_analyzer_h
