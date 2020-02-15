lib_LTLIBRARIES = libxmlcc.la

libxmlcc_la_SOURCES = parser.cxx qname.cxx serializer.cxx content.cxx \
  parser.hxx parser.txx qname.hxx serializer.hxx content.hxx exception.hxx \
  value_traits.hxx detail/export.hxx detail/macros.hxx detail/fwd.hxx

libxmlcc_la_CPPFLAGS = -I$(top_srcdir)

if !XMLCC_STATIC_LIB
  libxmlcc_la_CPPFLAGS += -DXMLCC_DYNAMIC_LIB
endif

if !XMLCC_USE_EXTERNAL_EXPAT
  libxmlcc_la_SOURCES += detail/expat/xmlparse.c \
    detail/expat/xmltok.c \
    detail/expat/xmltok.h \
    detail/expat/xmltok_ns.c \
    detail/expat/xmlrole.c \
    detail/expat/xmlrole.h \
    detail/expat/expat.h \
    detail/expat/ascii.h \
    detail/expat/expat_config.h \
    detail/expat/expat_external.h \
    detail/expat/winconfig.h \
    detail/expat/internal.h \
    detail/expat/siphash.h \
    detail/expat/nametab.h \
    detail/expat/xmltok_impl.h \
    detail/expat/xmltok_impl.c \
    detail/expat/utf8tab.h \
    detail/expat/latin1tab.h \
    detail/expat/asciitab.h \
    detail/expat/iasciitab.h
  libxmlcc_la_CPPFLAGS += "-DHAVE_EXPAT_CONFIG_H"
endif

if !XMLCC_USE_EXTERNAL_GENX
  libxmlcc_la_SOURCES += detail/genx/genx.c detail/genx/charProps.c detail/genx/genx.h
endif

libxmlcc_la_LDFLAGS = -version-info 0:0:0

