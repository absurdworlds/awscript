
aw_add_library(awscript-lang STATIC
	HEADERS
		include/aw/script/lexer/lexer.h
		include/aw/script/lexer/source_buffer.h
		include/aw/script/lexer/token.h
		include/aw/script/lexer/tokens.h
	SOURCES
		source/lexer/lexer.c++)


target_link_libraries(awscript-lang
	PUBLIC
		aw::io)


if (AWSCRIPT_MAKE_TESTS)
	add_subdirectory(tests)
endif()
