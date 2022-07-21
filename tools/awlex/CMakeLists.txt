
add_executable(awlex awlex.c++)

target_link_libraries(awlex
	PRIVATE
		aw::io
		aw::utility
		awscript-lang)
