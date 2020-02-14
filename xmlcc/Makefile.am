lib_LTLIBRARIES = libxmlcc.la

libxmlcc_la_SOURCES = parser.cxx qname.cxx serializer.cxx content.cxx

libxmlcc_la_CPPFLAGS =

if !XMLCC_USE_EXTERNAL_EXPAT
  libxmlcc_la_SOURCES += detail/expat/xmlparse.c \
    detail/expat/xmltok.c \
    detail/expat/xmltok_ns.c \
    detail/expat/xmlrole.c
    
  libxmlcc_la_CPPFLAGS += "-DHAVE_EXPAT_CONFIG_H"
endif

if !XMLCC_USE_EXTERNAL_GENX
  libxmlcc_la_SOURCES += detail/genx/genx.c detail/genx/charProps.c
endif

libxmlcc_la_LDFLAGS = -version-info 0:0:0

MAINTAINERCLEANFILES = Makefile.in
