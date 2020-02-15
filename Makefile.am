SUBDIRS = xmlcc example

ACLOCAL_AMFLAGS = -I m4

nobase_include_HEADERS = xmlcc/parser.hxx \
			 xmlcc/parser.txx \
			 xmlcc/exception.hxx \
			 xmlcc/serializer.hxx \
			 xmlcc/value_traits.hxx \
			 xmlcc/content.hxx \
			 xmlcc/qname.hxx \
			 xmlcc/detail/fwd.hxx \
			 xmlcc/detail/macros.hxx \
			 xmlcc/detail/config.hxx \
			 xmlcc/detail/export.hxx \
			 xmlcc/detail/config-vc.h \
			 $(builddir)/xmlcc/detail/config.h

if !XMLCC_USE_EXTERNAL_EXPAT
  nobase_include_HEADERS += xmlcc/detail/expat/expat.h \
			    xmlcc/detail/expat/expat_external.h
  
endif


if !XMLCC_USE_EXTERNAL_GENX
  nobase_include_HEADERS += xmlcc/detail/genx/genx.h
endif

