# CMakeList.txt : CMake project for xmlcc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (XMLXX_NO_EXCEPTIONS)
	add_compile_options(-fno-exceptions)
endif ()

configure_file(detail/config.hxx.in xmlcc/detail/config.hxx)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_compile_options(-Wall -Wextra)

# Add source to this project's executable.
add_library (xmlcc "parser.cxx" "parser.hxx" "parser.txx"
	"exception.cxx" "exception.hxx" "qname.cxx" "qname.hxx"
	"content.cxx" "serializer.hxx" "serializer.cxx"
	"detail/genx/genx.c"
	"detail/genx/charProps.c")

target_include_directories(xmlcc 
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	PRIVATE
		"detail/genx"
)

target_link_libraries(xmlcc EXPAT::EXPAT)

set_target_properties(xmlcc PROPERTIES
	VERSION 0.1.0
	SOVERSION 0)

include(GenerateExportHeader)
generate_export_header(xmlcc EXPORT_FILE_NAME xmlcc/detail/export.h)

install(TARGETS xmlcc DESTINATION ${LIBRARY_INSTALL_DIR})

install(FILES
	exception.hxx
	parser.hxx
	parser.txx
	serializer.hxx
	qname.hxx
	value_traits.hxx
	content.hxx
	DESTINATION include/xmlcc)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/xmlcc/detail/config.hxx
	${CMAKE_CURRENT_BINARY_DIR}/xmlcc/detail/export.h
	detail/fwd.hxx
	detail/macros.hxx
	DESTINATION include/xmlcc/detail)

install(FILES
	detail/genx/genx.h
	DESTINATION include/xmlcc/detail/genx)
