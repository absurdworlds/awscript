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
	case token_kind::less:
		return binop::less;
	case token_kind::greater:
		return binop::greater;
	case token_kind::equal_equal:
		return binop::equal;
	case token_kind::bang_equal:
		return binop::not_equal;
	case token_kind::equal:
		return binop::assignment;
	case token_kind::dot:
		return binop::access;
	default:
		return std::nullopt;
	}
}

auto parser::parse_binary_operator(token tok) -> std::optional<ast::binary_operator>
{
	auto result = token_to_binary_operator(tok);
	if (result)
		advance();
	return result;
}

} // namespace aw::script
