// file      : xml/details/export.hxx
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef XML_DETAILS_EXPORT_HXX
#define XML_DETAILS_EXPORT_HXX

#include <xmlcc/detail/config.hxx>

#ifdef XMLCC_STATIC_LIB
#  define XMLCC_EXPORT
#else
#  ifdef _WIN32
#    ifdef _MSC_VER
#      ifdef XMLCC_DYNAMIC_LIB
#        define XMLCC_EXPORT __declspec(dllexport)
#      else
#        define XMLCC_EXPORT __declspec(dllimport)
#      endif
#    else
#      ifdef XMLCC_DYNAMIC_LIB
#        ifdef DLL_EXPORT
#          define XMLCC_EXPORT __declspec(dllexport)
#        else
#          define XMLCC_EXPORT
#        endif
#      else
#        define XMLCC_EXPORT __declspec(dllimport)
#      endif
#    endif
#  else
#    define XMLCC_EXPORT
#  endif
#endif

#endif // XML_DETAILS_EXPORT_HXX
