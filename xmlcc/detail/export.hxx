#ifndef XMLCC_DETAIL_EXPORT_HXX
#define XMLCC_DETAIL_EXPORT_HXX

#ifdef XMLCC_BUILD_DLL
#ifdef _WIN32
#define XMLCC_EXPORT __declspec(dllexport)
#else
#define XMLCC_EXPORT __attribute__((visibility = "default"))
#endif // _WIN32
#else
#ifndef XMLCC_EXPORT
#ifdef _WIN32
#define XMLCC_EXPORT __declspec(dllimport)
#else
#define XMLCC_EXPORT
#endif // _WIN32
#endif // !XMLCC_EXPORT
#endif // XMLCC_BUILD_DLL
#endif
