#ifndef aw_script_parser_errors_h
#define aw_script_parser_errors_h

#include <optional>

#include <aw/script/diag/error_t.h>

namespace aw::script {

inline error_t error_not_implemented_yet(diagnostics_engine& diag, token tok)
{
	return error(diag, diagnostic_id::not_implemented_yet, tok);
}

inline error_t error_unexpected_token(diagnostics_engine& diag, token tok, token_kind expected)
{
	return error(diag, diagnostic_id::unexpected_token, tok, expected);
}

inline error_t error_expected_expression(diagnostics_engine& diag, token tok)
{
	return error(diag, diagnostic_id::expected_expression, tok);
}
} // namespace aw::script

#endif//aw_script_parser_errors_h
