#ifndef aw_script_semantic_analyzer_h
#define aw_script_semantic_analyzer_h

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/middle/declaration.h>
#include <aw/script/diag/diagnostics_engine.h>

namespace aw::script {

class scope;
class context;

// TODO: split into "ast_lowering" and "type_check"
class semantic_analyzer {
public:
	semantic_analyzer(diagnostics_engine& diag);

	auto lower(const ast::module& in_mod) -> middle::module;


private:
	// TODO: should be stored in the builtin_module, not here
	std::vector<std::unique_ptr<ir::type>> builtin_types;

	diagnostics_engine& diag;

	void visit(context& ctx, middle::declaration& decl);
	void visit(context& ctx, middle::function& decl);
	void visit(context& ctx, middle::variable& var);
	void visit(context& ctx, middle::struct_decl& decl);

	void visit_stmt(context& ctx, middle::statement& in_stmt);
	void visit_stmt(context& ctx, middle::statement_block& in_stmt);
	void visit_stmt(context& ctx, middle::decl_statement& in_stmt);
	void visit_stmt(context& ctx, middle::if_else_statement& in_stmt);
	void visit_stmt(context& ctx, middle::while_statement& in_stmt);
	void visit_stmt(context& ctx, middle::return_statement& in_stmt);
	void visit_stmt(context& ctx, middle::empty_statement& in_stmt);
	void visit_stmt(context& ctx, middle::expression& in_stmt);

	void visit_expr(context& ctx, middle::expression& in_expr);
	void visit_expr(context& ctx, middle::cast_expression& expr);
	void visit_expr(context& ctx, middle::unary_expression& in_expr);
	void visit_expr(context& ctx, middle::binary_expression& in_expr);
	void visit_expr(context& ctx, middle::call_expression& in_expr);
	void visit_expr(context& ctx, middle::field_expression& in_expr);
	void visit_expr(context& ctx, middle::if_expression& in_expr);
	void visit_expr(context& ctx, middle::value_expression& in_expr);
	void visit_expr(context& ctx, middle::numeric_literal& in_expr) {}
	void visit_expr(context& ctx, middle::string_literal& in_expr) {}
	void visit_expr(context& ctx, middle::struct_literal& in_expr) {}
	void visit_expr(context& ctx, middle::bool_literal& in_expr) {}

	auto common_type(ir::type* a, ir::type* b) -> ir::type*;

	auto common_type(context& ctx, middle::expression& lhs, middle::expression& rhs) -> ir::type*;
	auto common_type(context& ctx, ir::type* type, middle::expression& lhs, middle::expression& rhs) -> ir::type*;

	auto infer_type(context& ctx, middle::expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::cast_expression& expr) -> ir::type*;
	auto infer_type(context& ctx, middle::unary_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::binary_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::call_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::field_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::if_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::value_expression& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::numeric_literal& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::bool_literal& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::string_literal& in_expr) -> ir::type*;
	auto infer_type(context& ctx, middle::struct_literal& in_expr) -> ir::type*;

	auto propagate_type(context& ctx, ir::type* type, middle::expression& expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::cast_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::unary_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::binary_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::call_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::field_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::if_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::value_expression& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::numeric_literal& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::bool_literal& expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::string_literal& in_expr) -> ir::type*;
	auto propagate_type(context& ctx, ir::type* type, middle::struct_literal& in_expr) -> ir::type*;
};


} // namespace aw::script

#endif // aw_script_semantic_analyzer_h
