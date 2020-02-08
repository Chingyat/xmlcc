# CMakeList.txt : CMake project for xmlcc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_compile_definitions(XMLCC_BUILD_DLL)


# Add source to this project's executable.
add_library (xmlcc SHARED "parser.cxx" "parser.hxx" 
	"exception.cxx" "exception.hxx" "qname.cxx" "qname.hxx")


target_link_libraries(xmlcc EXPAT::EXPAT)
