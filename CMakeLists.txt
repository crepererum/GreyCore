cmake_minimum_required (VERSION 2.8)

project (GreyCore)

# include modules
include (CheckIncludeFileCXX)

# configure compiler
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")

# detect clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -fdiagnostics-show-category=name")
endif ()

# set include dirs
include_directories ("include/")

# detect cxxabi.h
check_include_file_cxx ("cxxabi.h" HAVE_CXXABI_H)
if (HAVE_CXXABI_H)
	add_definitions (-DHAVE_CXXABI_H)
endif ()

# build/install libgreycore
aux_source_directory (src/libgreycore GCSourcesLib)
add_library (greycore ${GCSourcesLib})
target_link_libraries (greycore pthread)
install (DIRECTORY "include/" DESTINATION "include/greycore")
install (TARGETS greycore DESTINATION "lib")

# build/install utils
aux_source_directory (src/utils GCSourcesUtils)
foreach (fname ${GCSourcesUtils})
	get_filename_component (exe ${fname} NAME_WE)
	add_executable (${exe} ${fname})
	target_link_libraries (${exe} greycore)
	install (TARGETS ${exe} DESTINATION "bin")
endforeach ()

