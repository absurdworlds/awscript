/*
 * Copyright (C) 2015-2022  Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>

#include <aw/algorithm/in.h>
#include <aw/io/file.h>
#include <aw/io/read_file.h>
#include <aw/script/lexer/lexer.h>
#include <aw/script/utility/print_token.h>
namespace aw::script {
void print_token_info(token& token)
{
	const bool show_data = in(token,
	                          token_kind::identifier,
	                          token_kind::numeric_constant,
	                          token_kind::string_literal);

	std::string msg = name_token(token.kind);
	if (show_data) {
		msg += ", ";
		msg += token.data;
	}

	// TODO: use print from awtest
	std::cout << "〈" << msg << "〉";
}

void print_tokens(source_buffer& buffer, lexer& lexer)
{
	token prev;
	auto token = lexer.current();
	do
	{
		if (prev.kind != token_kind::none)
		{
			// TODO: add helper to get whitespace between tokens
			const auto start = buffer.begin() + prev.loc.pos + prev.data.size();
			const auto end   = buffer.begin() + token.loc.pos;
			std::cout << std::string_view(start, end);
		}
		print_token_info(token);
		prev = token;
		token = lexer.next();
	} while(token != token_kind::eof);
}
} // namespace aw::script

namespace awlex {
enum class output_format {
	pretty,
	json, // TODO
	awdoc // TODO
};

struct options {
	output_format format = output_format::pretty;
	std::string_view file_name;
};

// TODO: use proper arg parser
enum parse_result {
	skip,
	ok,
	error
};

parse_result parse_option(options& options, aw::string_view option)
{
	using namespace std::string_view_literals;
	if (option.starts_with("--format="))
	{
		option.remove_prefix("--format="sv.size());
		if (option == "pretty")
		{
			options.format = output_format::pretty;
			return ok;
		}
		else
		{
			std::cerr << "Unrecognized format: " << option << '\n';
			return error;
		}
	}
	return skip;
}

bool parse_args(options& options, int argc, char** args)
{
	using namespace std::string_view_literals;
	if (argc < 2)
		return false;

	bool skip_options = false;

	for (int i = 1; i < argc; ++i)
	{
		auto arg = aw::string_view(args[i]);
		if (!skip_options && arg.starts_with('-'))
		{
			if (arg == "--")
			{
				skip_options = true;
				continue;
			}
			const auto result = parse_option(options, arg);
			if (result == ok)
				continue;
			if (result == error)
				return false;
		}

		if (options.file_name.empty())
		{
			options.file_name = arg;
		}
	}

	return true;
}
} // namespace awlex

int main(int argc, char** args)
{
	using namespace aw;
	using namespace aw::script;

	awlex::options options;

	if (!awlex::parse_args(options, argc, args))
		return 2;

	if (options.file_name.empty())
		return 3;

	io::read_file<io::file> file(options.file_name);
	source_buffer buffer(file);
	lexer lexer(&buffer);

	switch (options.format)
	{
	case awlex::output_format::pretty:
		aw::script::print_tokens(buffer, lexer);
		break;
	case awlex::output_format::awdoc:
		break;
	case awlex::output_format::json:
		break;
	}

}
