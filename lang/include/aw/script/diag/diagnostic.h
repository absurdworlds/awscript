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
#include <vector>

#include <aw/script/utility/location.h>
#include <aw/script/utility/print_token.h>
#include <aw/script/ast/identifier.h>
namespace aw::script {
class diagnostics_engine;

enum class diagnostic_id {
#define DIAG(type, id, msg) id,
#include <aw/script/diag/messages.h>
};

/*!
 * This class is used to compose diagnostic messages
 */
class diagnostic {
public:
	diagnostic(location loc, diagnostic_id id)
		: loc(loc), id(id)
	{
	}

	diagnostic& arg(std::string str)
	{
		args.push_back(std::move(str));
		return *this;
	}
private:
	friend class diagnostics_engine;

	location loc;
	diagnostic_id id;
	std::vector<std::string> args;
};

std::string diag_message_template(diagnostic_id id);

inline diagnostic& operator<<(diagnostic& diag, std::string str)
{
	return diag.arg(str);
}

inline diagnostic& operator<<(diagnostic& diag, std::string_view str)
{
	return diag.arg(std::string(str));
}

inline diagnostic& operator<<(diagnostic& diag, token_kind type)
{
	return diag.arg(spell_token(type));
}

inline diagnostic& operator<<(diagnostic& diag, const ast::identifier& id)
{
	return diag.arg(to_string(id));
}
} // namespace aw::script
#endif//aw_script_diagnostic_h
