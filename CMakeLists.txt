cmake_minimum_required (VERSION 2.8)

project (GreyCore)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")

# detect clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -fdiagnostics-show-category=name")
endif ()

include_directories ("include/")

# build libgreycore
aux_source_directory (src/libgreycore GCSourcesLib)
add_library (greycore ${GCSourcesLib})
target_link_libraries (greycore pthread)

# build utils
aux_source_directory (src/utils GCSourcesUtils)
foreach (fname ${GCSourcesUtils})
	get_filename_component (exe ${fname} NAME_WE)
	add_executable (${exe} ${fname})
	target_link_libraries (${exe} greycore)
endforeach ()

