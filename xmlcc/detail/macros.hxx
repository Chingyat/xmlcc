#ifndef XMLCC_DETAIL_MACROS_HXX
#define XMLCC_DETAIL_MACROS_HXX

#include <xmlcc/detail/config.hxx>

#ifndef XMLXX_NOEXCEPT
#ifdef XMLXX_CXX11
#define XMLXX_NOEXCEPT noexcept
#else
#define XMLXX_NOEXCEPT throw()
#endif
#endif

#ifndef XMLXX_TRY
#ifndef XMLXX_NO_EXCEPTIONS
#define XMLXX_TRY try
#else
#define XMLXX_TRY
#endif
#endif

#ifndef XMLXX_CONSTEXPR
#ifdef XMLXX_CXX11
#define XMLXX_CONSTEXPR constexpr
#else
#define XMLXX_CONSTEXPR
#endif
#endif

#ifndef XMLXX_CATCH
#ifndef XMLXX_NO_EXCEPTIONS
#define XMLXX_CATCH catch
#else
#define XMLXX_CATCH(...) if (false)
#endif
#endif

#ifndef XMLXX_THROW
#ifndef XMLXX_NO_EXCEPTIONS
#define XMLXX_THROW throw
#else
#define XMLXX_THROW std::terminate(),
#endif
#endif

#ifndef XMLXX_RETHROW
#ifndef XMLXX_NO_EXCEPTIONS
#define XMLXX_RETHROW throw
#else
#define XMLXX_RETHROW std::terminate()
#endif
#endif

#ifdef XMLXX_CXX11
#define XMLXX_MOVE(x) std::move(x)
#else
#define XMLXX_MOVE(x) x
#endif

#ifndef XMLXX_STRING_CONST_REF
#ifdef XMLXX_CXX17
#define XMLXX_STRING_CONST_REF std::string_view
#else
#define XMLXX_STRING_CONST_REF const std::string &
#endif
#endif

#endif // !XMLCC_DETAIL_MACROS_HXX
