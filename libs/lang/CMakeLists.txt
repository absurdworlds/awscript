
aw_add_library(awscript-lang STATIC
	HEADERS
		include/aw/script/utility/location.h
		include/aw/script/lexer/lexer.h
		include/aw/script/lexer/source_buffer.h
		include/aw/script/lexer/token.h
		include/aw/script/lexer/tokens.h
		include/aw/script/utility/print_token.h
		include/aw/script/ast/declaration.h
		include/aw/script/ast/decl/class.h
		include/aw/script/ast/decl/function.h
		include/aw/script/ast/decl/variable.h
		include/aw/script/ast/decl/type.h
		include/aw/script/ast/statement.h
		include/aw/script/ast/expression.h
		include/aw/script/diag/diagnostic.h
		include/aw/script/diag/diagnostics_engine.h
		include/aw/script/diag/messages.h
		include/aw/script/parser/parser.h
		include/aw/script/parser/operators.h
		include/aw/script/symtab/scope.h
		include/aw/script/symtab/symbol_table.h
		include/aw/script/symtab/symbol_name.h
	SOURCES
		source/lexer/lexer.c++
		source/ast/function.c++
		source/diag/messages.c++
		source/diag/diagnostics_engine.c++
		source/parser/parser.c++
		source/parser/operators.c++
		source/parser/errors.h
		source/symtab/scope.c++
		source/symtab/symbol_table.c++
	)

target_link_libraries(awscript-lang
	PUBLIC
		aw::io
		aw::string)

if (AWSCRIPT_MAKE_TESTS)
	add_subdirectory(tests)
endif()
