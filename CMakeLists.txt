cmake_minimum_required(VERSION 3.19)

project(awscript VERSION 0)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
	# MSVC requires /std:c++latest for some C++20 features
	set(CMAKE_CXX_STANDARD 23)
else()
	set(CMAKE_CXX_STANDARD 20)
endif()

set(AW_IMPORT_MACROS ON)
find_package(awlib)

option(AWSCRIPT_MAKE_TESTS   "Build test executables" ON)
option(AWSCRIPT_ENABLE_TESTS "Enable running the tests" ON)

if (AWSCRIPT_ENABLE_TESTS)
	enable_testing()
endif()


add_subdirectory(lang)
add_subdirectory(tools)

if (AWSCRIPT_ENABLE_TESTS)
	add_subdirectory(test)
endif()
