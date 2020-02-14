
#ifndef XMLCC_EXPORT_H
#define XMLCC_EXPORT_H

#ifdef XMLCC_STATIC_DEFINE
#  define XMLCC_EXPORT
#  define XMLCC_NO_EXPORT
#else
#  ifndef XMLCC_EXPORT
#    ifdef xmlcc_EXPORTS
        /* We are building this library */
#      define XMLCC_EXPORT 
#    else
        /* We are using this library */
#      define XMLCC_EXPORT 
#    endif
#  endif

#  ifndef XMLCC_NO_EXPORT
#    define XMLCC_NO_EXPORT 
#  endif
#endif

#ifndef XMLCC_DEPRECATED
#  define XMLCC_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef XMLCC_DEPRECATED_EXPORT
#  define XMLCC_DEPRECATED_EXPORT XMLCC_EXPORT XMLCC_DEPRECATED
#endif

#ifndef XMLCC_DEPRECATED_NO_EXPORT
#  define XMLCC_DEPRECATED_NO_EXPORT XMLCC_NO_EXPORT XMLCC_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef XMLCC_NO_DEPRECATED
#    define XMLCC_NO_DEPRECATED
#  endif
#endif

#endif /* XMLCC_EXPORT_H */
