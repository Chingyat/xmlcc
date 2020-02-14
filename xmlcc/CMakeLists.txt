# CMakeList.txt : CMake project for xmlcc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (XMLXX_NO_EXCEPTIONS)
	add_compile_options(-fno-exceptions)
endif ()

if (WIN32)
	add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
endif()

configure_file(detail/config.hxx.in xmlcc/detail/config.hxx)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library (my_genx OBJECT EXCLUDE_FROM_ALL "detail/genx/genx.c" "detail/genx/charProps.c")

aux_source_directory ("detail/expat" EXPAT_SOURCES)
add_library (my_expat OBJECT EXCLUDE_FROM_ALL ${EXPAT_SOURCES})
target_compile_definitions (my_expat PUBLIC "XML_STATIC")
if (NOT WIN32)
  target_compile_definitions(my_expat PRIVATE -DHAVE_EXPAT_CONFIG_H=1)
endif ()
target_include_directories (my_expat PRIVATE "detail/expat")

# Add source to this project's executable.
add_library (xmlcc "parser.cxx" "parser.hxx" "parser.txx"
	"exception.hxx" "qname.cxx" "qname.hxx"
	"content.cxx" "serializer.hxx" "serializer.cxx")

get_target_property(position_independent_code xmlcc POSITION_INDEPENDENT_CODE)

set_property(TARGET my_expat my_genx PROPERTY POSITION_INDEPENDENT_CODE ${position_independent_code})

target_include_directories(xmlcc 
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	PRIVATE
		"detail/genx"
)

if (XMLCC_USE_EXTERNAL_EXPAT)
    target_link_libraries(xmlcc EXPAT::EXPAT)
else ()
    target_link_libraries(xmlcc "my_expat")
endif ()

if (XMLCC_USE_EXTERNAL_GENX)
    target_link_libraries(xmlcc "genx")
else ()
    target_link_libraries(xmlcc "my_genx")
endif ()

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

if (NOT XMLCC_USE_EXTERNAL_GENX)
	install (FILES
		detail/genx/genx.h
		DESTINATION include/xmlcc/detail/genx)
endif ()

if (NOT XMLCC_USE_EXTERNAL_EXPAT)
	install (FILES
		detail/expat/expat.h
		detail/expat/expat_external.h
		DESTINATION include/xmlcc/detail/expat)
endif ()
