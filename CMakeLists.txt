cmake_minimum_required (VERSION 2.8)

project (GreyCore)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")

aux_source_directory (src GCSources)
add_executable (greycore ${GCSources})
target_link_libraries (greycore pthread)

