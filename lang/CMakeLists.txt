
aw_add_library(awscript-lang STATIC
	HEADERS
		include/aw/script/utility/location.h
		include/aw/script/lexer/lexer.h
		include/aw/script/lexer/source_buffer.h
		include/aw/script/lexer/token.h
		include/aw/script/lexer/tokens.h
		include/aw/script/utility/print_token.h
		include/aw/script/ast/declaration.h
		include/aw/script/ast/decl/function.h
		include/aw/script/ast/decl/variable.h
		include/aw/script/ast/decl/type.h
		include/aw/script/diag/diagnostic.h
		include/aw/script/diag/diagnostics_engine.h
		include/aw/script/diag/messages.h
		include/aw/script/parser/parser.h
	SOURCES
		source/lexer/lexer.c++
		source/diag/messages.c++
		source/diag/diagnostics_engine.c++
		source/parser/parser.c++)


target_link_libraries(awscript-lang
	PUBLIC
		aw::io)


if (AWSCRIPT_MAKE_TESTS)
	add_subdirectory(tests)
endif()
