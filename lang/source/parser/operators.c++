#include <aw/script/parser/parser.h>
#include <aw/script/parser/operators.h>

#include <aw/script/ast/expression.h>

namespace aw::script {

auto static token_to_unary_operator(token tok) -> std::optional<ast::unary_operator>
{
	switch (tok.kind) {
	case token_kind::minus:
		return ast::unary_operator::minus;
	case token_kind::plus:
		return ast::unary_operator::plus;
	case token_kind::bang:
		return ast::unary_operator::logical_negation;
	case token_kind::tilde:
		return ast::unary_operator::binary_negation;
	default:
		return std::nullopt;
	}
}

auto parser::parse_unary_operator(token tok) -> std::optional<ast::unary_operator>
{
	auto result = token_to_unary_operator(tok);
	if (result)
		advance();
	return result;
}

auto static token_to_binary_operator(token tok) -> std::optional<ast::binary_operator>
{
	using binop = ast::binary_operator;

	switch (tok.kind) {
	case token_kind::minus:
		return binop::minus;
	case token_kind::plus:
		return binop::plus;
	case token_kind::ast:
		return binop::multiply;
	case token_kind::slash:
		return binop::divide;
	case token_kind::percent:
		return binop::modulo;

	case token_kind::amp:
		return binop::bitwise_and;
	case token_kind::caret:
		return binop::bitwise_xor;
	case token_kind::pipe:
		return binop::bitwise_or;
	case token_kind::less_less:
		return binop::shift_left;
	case token_kind::greater_greater:
		return binop::shift_right;

	case token_kind::less:
		return binop::less;
	case token_kind::less_equal:
		return binop::less_equal;
	case token_kind::greater:
		return binop::greater;
	case token_kind::greater_equal:
		return binop::greater_equal;
	case token_kind::equal_equal:
		return binop::equal;
	case token_kind::bang_equal:
		return binop::not_equal;

	case token_kind::pipe_pipe:
		return binop::logical_or;
	case token_kind::amp_amp:
		return binop::logical_and;

	case token_kind::equal:
		return binop::assignment;
	case token_kind::minus_equal:
		return binop::assign_minus;
	case token_kind::plus_equal:
		return binop::assign_plus;
	case token_kind::ast_equal:
		return binop::assign_multiply;
	case token_kind::slash_equal:
		return binop::assign_divide;
	case token_kind::percent_equal:
		return binop::assign_modulo;
	case token_kind::pipe_equal:
		return binop::assign_or;
	case token_kind::caret_equal:
		return binop::assign_xor;
	case token_kind::amp_equal:
		return binop::assign_and;
	case token_kind::less_less_equal:
		return binop::assign_shift_left;
	case token_kind::greater_greater_equal:
		return binop::assign_shift_right;

	case token_kind::dot:
		return binop::access;
	case token_kind::dot_dot:
		return binop::range;
	default:
		break;
	}

	return std::nullopt;
}

auto parser::parse_binary_operator(token tok) -> std::optional<ast::binary_operator>
{
	auto result = token_to_binary_operator(tok);
	if (result)
		advance();
	return result;
}

} // namespace aw::script
