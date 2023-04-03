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

	auto lower(const ast::module& in_mod) -> middle::module;


private:
	// TODO: should be stored in the builtin_module, not here
	std::vector<std::unique_ptr<ast::type>> builtin_types;

	void visit(context& ctx, middle::declaration& decl);
	void visit(context& ctx, middle::function& decl);
	void visit(context& ctx, middle::variable& var);

	void visit_stmt(context& ctx, middle::statement& in_stmt);
	void visit_stmt(context& ctx, middle::statement_block& in_stmt);
	void visit_stmt(context& ctx, middle::if_else_statement& in_stmt);
	void visit_stmt(context& ctx, middle::return_statement& in_stmt);
	void visit_stmt(context& ctx, middle::empty_statement& in_stmt);
	void visit_stmt(context& ctx, middle::expression& in_stmt);

	void visit_expr(context& ctx, middle::expression& in_expr);
	void visit_expr(context& ctx, middle::unary_expression& in_expr);
	void visit_expr(context& ctx, middle::binary_expression& in_expr);
	void visit_expr(context& ctx, middle::call_expression& in_expr);
	void visit_expr(context& ctx, middle::if_expression& in_expr);
	void visit_expr(context& ctx, middle::value_expression& in_expr);
	void visit_expr(context& ctx, middle::numeric_literal& in_expr);
	void visit_expr(context& ctx, middle::string_literal& in_expr);

	void visit_op(context& ctx, ast::type* ty, middle::binary_expression& expr);


	auto common_type(ast::type* a, ast::type* b) -> ast::type*;

	auto common_type(context& ctx, middle::expression& lhs, middle::expression& rhs) -> ast::type*;
	auto common_type(context& ctx, ast::type* type, middle::expression& lhs, middle::expression& rhs) -> ast::type*;

	auto infer_type(context& ctx, middle::expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::unary_expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::binary_expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::call_expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::if_expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::value_expression& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::numeric_literal& in_expr) -> ast::type*;
	auto infer_type(context& ctx, middle::string_literal& in_expr) -> ast::type*;

	auto propagate_type(context& ctx, ast::type* type, middle::expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::unary_expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::binary_expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::call_expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::if_expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::value_expression& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::numeric_literal& in_expr) -> ast::type*;
	auto propagate_type(context& ctx, ast::type* type, middle::string_literal& in_expr) -> ast::type*;
};


} // namespace aw::script

#endif // aw_script_semantic_analyzer_h
