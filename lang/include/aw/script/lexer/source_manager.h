/*
 * Copyright (C) 2023 Hudd <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_script_source_manager_h
#define aw_script_source_manager_h

// TODO: hide this include
#include <aw/script/lexer/source_buffer.h>

#include <unordered_map>

namespace aw::script {

class source_manager {
public:
	source_buffer* get_buffer(size_t id)
	{
		if (id < files.size())
			return &files[id];
		return nullptr;
	}

	const source_buffer* get_buffer(size_t id) const
	{
		if (id < files.size())
			return &files[id];
		return nullptr;
	}

	size_t add_buffer(std::string_view content, fs::path path = {})
	{
		const auto id = files.size();
		files.emplace_back(content, id, std::move(path));
		return id;
	}

	size_t add_file(const fs::path& path)
	{
		const auto id = files.size();
		io::read_file<io::file> file(path);
		files.emplace_back(file, files.size());
		return id;
	}

private:
	std::vector<source_buffer> files;
};


} // namespace aw::script

#endif//aw_script_source_manager_h
