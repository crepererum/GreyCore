cmake_minimum_required (VERSION 2.8)

project (GreyList)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")

aux_source_directory (src GLSources)
add_executable (greylist ${GLSources})
target_link_libraries (greylist pthread)

