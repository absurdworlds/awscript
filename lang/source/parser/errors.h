#ifndef aw_script_parser_errors_h
#define aw_script_parser_errors_h

#include <aw/script/diag/diagnostics_engine.h>

namespace aw::script {
template<typename...Args>
std::nullptr_t error(diagnostics_engine& diag, diagnostic_id id, token tok, Args&&... args)
{
	diagnostic msg(tok.loc, id);
	msg << tok.data;
	if constexpr(sizeof...(args) > 0)
		(msg << ... << args);
	diag.report(msg);

	return nullptr;
}

std::nullptr_t
error_not_implemented_yet(diagnostics_engine& diag, token tok)
{
	return error(diag, diagnostic_id::not_implemented_yet, tok);
}

std::nullptr_t
error_unexpected_token(diagnostics_engine& diag, token tok, token_kind expected)
{
	return error(diag, diagnostic_id::unexpected_token, tok, expected);
}
} // namespace aw::script

#endif//aw_script_parser_errors_h
