#include <aw/script/driver/main.h>

#include <aw/utility/argv_parser.h>
#include <aw/algorithm/in.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
	using aw::in;
	std::vector<std::string> parameters;

	aw::utils::argv_parser parser(argv+1);

	aw::script::driver::options options;
	while(true) {
		auto arg = parser.parse_argument();
		if (!arg)
			break;

		if (arg->type == arg->argument){
			options.input_files.push_back(arg->value);
			continue;
		}

		auto name = arg->name;
		auto value = arg->value;

		if (name.empty())
			return 1;
		if (in(name, "o", "output")) {
			if (value.empty())
				value = parser.get_param();
			options.output_file = value;
		} else if (in(name, "dump-ir")) {
			options.dump_ir = true;
		}
	}

	if (options.output_file.empty())
		options.output_file = "a.out";

	return aw::script::driver::run_compiler(options);
}
