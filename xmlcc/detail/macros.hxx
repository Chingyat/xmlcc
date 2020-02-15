#ifndef XMLCC_DETAIL_MACROS_HXX
#define XMLCC_DETAIL_MACROS_HXX

#include <xmlcc/detail/config.hxx>

#ifndef XMLCC_NOEXCEPT
#ifdef XMLCC_CXX11
#define XMLCC_NOEXCEPT noexcept
#else
#define XMLCC_NOEXCEPT throw()
#endif
#endif

#ifndef XMLCC_TRY
#ifndef XMLCC_NO_EXCEPTIONS
#define XMLCC_TRY try
#else
#define XMLCC_TRY
#endif
#endif

#ifndef XMLCC_CONSTEXPR
#ifdef XMLCC_CXX11
#define XMLCC_CONSTEXPR constexpr
#else
#define XMLCC_CONSTEXPR
#endif
#endif

#ifndef XMLCC_CATCH
#ifndef XMLCC_NO_EXCEPTIONS
#define XMLCC_CATCH catch
#else
#define XMLCC_CATCH(...) if (false)
#endif
#endif

#ifndef XMLCC_THROW
#ifndef XMLCC_NO_EXCEPTIONS
#define XMLCC_THROW throw
#else
#define XMLCC_THROW std::terminate(),
#endif
#endif

#ifndef XMLCC_RETHROW
#ifndef XMLCC_NO_EXCEPTIONS
#define XMLCC_RETHROW throw
#else
#define XMLCC_RETHROW std::terminate()
#endif
#endif

#ifdef XMLCC_CXX11
#define XMLCC_MOVE(x) std::move(x)
#else
#define XMLCC_MOVE(x) x
#endif

#ifndef XMLCC_STRING_CONST_REF
#ifdef XMLCC_CXX17
#define XMLCC_STRING_CONST_REF std::string_view
#else
#define XMLCC_STRING_CONST_REF const std::string &
#endif
#endif

#endif // !XMLCC_DETAIL_MACROS_HXX
