#ifndef XMLCC_DETAIL_EXPORT_HXX
#define XMLCC_DETAIL_EXPORT_HXX

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

#endif // XMLCC_DETAIL_EXPORT
