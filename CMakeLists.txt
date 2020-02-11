# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("xmlcc")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

option (XMLXX_CXX11 "Use C++11 features" ON)
option (XMLXX_CXX14 "Use C++14 features" ON)
option (XMLXX_CXX17 "Use C++17 features" ON)

option (XMLXX_NO_EXCEPTIONS "Disable exception handling" OFF)

find_package(EXPAT REQUIRED)

include_directories(.)

# Include sub-projects.
add_subdirectory ("xmlcc")
add_subdirectory ("example")
