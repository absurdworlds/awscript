
aw_add_library(awscript-llvm STATIC
	HEADERS
		source/backend_llvm.h
	SOURCES
		source/backend_llvm.c++
	)


find_package(LLVM REQUIRED CONFIG)

execute_process(COMMAND llvm-config --libs all
                OUTPUT_VARIABLE LLVM_LIBRARIES
                OUTPUT_STRIP_TRAILING_WHITESPACE)

target_link_libraries(awscript-llvm
	PUBLIC
		aw::io
		aw::string
		awscript-lang
		${LLVM_LIBRARIES})

if (AWSCRIPT_MAKE_TESTS)
	#add_subdirectory(tests)
endif()
