cmake_minimum_required(VERSION 3.19)

project(awscript VERSION 0)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (MSVC)
	# MSVC requires /std:c++latest for some C++20 features
	set(CMAKE_CXX_STANDARD 23)
else()
	set(CMAKE_CXX_STANDARD 20)
endif()

set(AW_IMPORT_MACROS ON)
find_package(awlib)

option(AWSCRIPT_MAKE_TESTS "Build test executables" ON)

add_subdirectory(lang)
