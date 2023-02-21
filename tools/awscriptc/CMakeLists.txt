
add_executable(awscriptc
	awscriptc.c++
)

target_link_libraries(awscriptc
	PRIVATE
		aw::io
		aw::utility
		awscript-lang)
