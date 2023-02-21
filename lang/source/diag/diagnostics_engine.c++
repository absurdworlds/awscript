#include <aw/script/diag/diagnostics_engine.h>

#include <aw/script/lexer/source_buffer.h>

#include <iostream> //temporary

namespace aw::script {

struct source_position {
	size_t line = 0;
	size_t column = 0;
};

source_position count_lines(const char* begin, const char* end, ptrdiff_t offset)
{
	auto pos  = begin;
	auto last = pos + offset;
	auto prev = pos;

	assert(last <= end);
	last = std::min(last, end);

	size_t row = 1;

	while (pos++ < last) {
		if (*pos == '\n') {
			prev = pos;
			++row;
		}
	}

	size_t column = last - prev;

	return {row, column};
}

source_position count_lines(source_buffer& buffer, ptrdiff_t offset)
{
	return count_lines(std::begin(buffer), std::end(buffer), offset);
}

void diagnostics_engine::report(diagnostic diag)
{
	array_view<std::string> args(diag.args);
	auto msg = aw::string::compose_v(diag_message_template(diag.id), args);

	auto line   = diag.loc.pos;
	auto column = 0;
	if (_buffer)
	{
		auto pos = count_lines(*_buffer, line);
		line = pos.line;
		column = pos.column;
	}

	std::cerr << "error:" << line << ":" << column << ": " << msg << "\n";
}

} // namespace aw::script
