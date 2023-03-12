
aw_add_library(awscript-lang STATIC
	HEADERS
		include/aw/script/utility/location.h
		include/aw/script/lexer/lexer.h
		include/aw/script/lexer/source_buffer.h
		include/aw/script/lexer/source_manager.h
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
		include/aw/script/diag/error_t.h
		include/aw/script/diag/messages.h
		include/aw/script/parser/parser.h
		include/aw/script/parser/operators.h
		include/aw/script/symtab/scope.h
		include/aw/script/symtab/symbol_table.h
		include/aw/script/symtab/symbol_name.h
		include/aw/script/codegen/backend.h
		include/aw/script/codegen/optimization_level.h
		include/aw/script/driver/main.h
		include/aw/script/driver/options.h
		include/aw/script/driver/callbacks.h
		source/codegen/llvm/backend_llvm.h
		source/codegen/llvm/optimizer_llvm.h
		source/codegen/llvm/llvm_helpers.h
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
		source/codegen/backend.c++
		source/codegen/llvm/backend_llvm.c++
		source/codegen/llvm/optimizer_llvm.c++
		source/driver/main.c++
	)

target_link_libraries(awscript-lang
	PUBLIC
		aw::io
		aw::string)

set (AWSCRIPT_USE_LLVM ON)
if (AWSCRIPT_USE_LLVM)
	find_package(LLVM REQUIRED CONFIG)

	option (AWSCRIPT_DEBUG_LLVM "Use a limited set of LLVM libraries for easier debugging" OFF)
	if (AWSCRIPT_DEBUG_LLVM)
		message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
		message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

		separate_arguments(LLVM_DEFINITIONS)

		llvm_map_components_to_libnames(LLVM_LIBRARIES
			support
			core
			target
			analysis
			passes
			x86desc
			x86asmparser
			x86codegen)

		message(STATUS "LLVM_LIBRARIES: ${LLVM_LIBRARIES}")
		message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
		message(STATUS "LLVM_TOOLS_BINARY_DIR: ${LLVM_TOOLS_BINARY_DIR}")
		#message(STATUS "LLVM_AVAILABLE_LIBS: ${LLVM_AVAILABLE_LIBS}")
		#message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")

		target_include_directories(awscript-lang
			PRIVATE
				${LLVM_INCLUDE_DIRS})

		target_compile_definitions(awscript-lang
			PRIVATE
				-DAWSCRIPT_DEBUG_LLVM=1)
	else()
		execute_process(COMMAND llvm-config --libs all
		                OUTPUT_VARIABLE LLVM_LIBRARIES
		                OUTPUT_STRIP_TRAILING_WHITESPACE)
	endif()

	target_link_libraries(awscript-lang
			PUBLIC
				${LLVM_LIBRARIES})
endif()


if (AWSCRIPT_MAKE_TESTS)
	add_subdirectory(tests)
endif()
