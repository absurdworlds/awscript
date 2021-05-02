#ifndef aw_script_parser_errors_h
#define aw_script_parser_errors_h

#include <aw/script/diagnostic/diagnostics_engine.h>

namespace aw::script {
template<typename...Args>
std::nullptr_t error(diagnostics_engine& diag, Diagnostic::ID id, Token tok, Args&&... args)
{
	Diagnostic msg(tok.location(), id);
	msg << tok.data();
	if constexpr(sizeof...(args) > 0)
		(msg << ... << args);
	diag.report(msg);

	return nullptr;
}

std::nullptr_t
error_not_implemented_yet(diagnostics_engine& diag, Token tok)
{
	return error(diag, Diagnostic::not_implemented_yet, tok);
}

std::nullptr_t
error_unexpected_token(diagnostics_engine& diag, Token tok, TokenType expected)
{
	return error(diag, Diagnostic::unexpected_token, tok, expected);
}
} // namespace aw::script

#endif//aw_script_parser_errors_h
