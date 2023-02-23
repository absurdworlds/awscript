#include <aw/script/driver/main.h>


int main(int argc, char** argv)
{
	if (argv[1] == nullptr)
		return 1;

	aw::script::driver::options options;
	options.input_files.push_back(argv[1]);
	options.output_file = argv[2] ? argv[2] : "a.out";

	return aw::script::driver::run_compiler(options);
}
