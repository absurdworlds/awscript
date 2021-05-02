/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_diagnostic_h
#define aw_script_diagnostic_h
#include <aw/script/utility/location.h>
#include <aw/script/utility/print_token.h>
namespace aw {
namespace script {
class diagnostics_engine;

/*!
 * This class is used to compose diagnostic messages
 */
class Diagnostic {
public:
	enum ID {
#define DIAG(type, id, msg) id,
#include <aw/script/diagnostic/messages.h>
#undef DIAG
	};

	Diagnostic(location loc, ID id)
		: id(id), loc(loc)
	{
	}

	Diagnostic& arg(std::string str)
	{
		args.push_back(str);
		return *this;
	}
private:
	friend class diagnostics_engine;

	location loc;
	ID id;
	std::vector<std::string> args;
};

std::string getDiagMsg(Diagnostic::ID id)
{
	switch (id) {
#define DIAG(type, id, msg) case Diagnostic::id: return msg;
#include <aw/script/diagnostic/messages.h>
#undef DIAG
	}
	return {};
}

Diagnostic& operator<<(Diagnostic& diag, std::string str)
{
	return diag.arg(str);
}

Diagnostic& operator<<(Diagnostic& diag, std::string_view str)
{
	return diag.arg(std::string(str));
}

Diagnostic& operator<<(Diagnostic& diag, TokenType type)
{
	return diag.arg(spellToken(type));
}
} // namespace script
} // namespace aw
#endif//aw_script_diagnostic_h
