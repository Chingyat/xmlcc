#ifndef XMLXX_DETAIL_CONFIG_HXX
#define XMLXX_DETAIL_CONFIG_HXX

#ifndef XMLXX_CXX17
#define XMLXX_CXX17 0
#endif

#ifndef XMLXX_CXX11
#define XMLXX_CXX11 1

#if XMLXX_CXX17
#define XMLXX_CXX11 1
#endif

#endif

#ifndef XMLXX_NO_EXCEPTION
#define XMLXX_NO_EXCEPTION 1
#endif

#ifndef XMLXX_NOEXCEPT
#if !XMLXX_NO_EXCEPTION
#define XMLXX_NOEXCEPT throw()
#else
#define XMLXX_NOEXCEPT
#endif
#endif

#ifndef XMLXX_TRY
#if !XMLXX_NO_EXCEPTION
#define XMLXX_TRY try
#else
#define XMLXX_TRY
#endif
#endif

#ifndef XMLXX_CATCH
#if !XMLXX_NO_EXCEPTION
#define XMLXX_CATCH catch
#else
#define XMLXX_CATCH(...) if (false)
#endif
#endif

#ifndef XMLXX_THROW
#if !XMLXX_NO_EXCEPTION
#define XMLXX_THROW(x) throw x
#else
#define XMLXX_THROW(...) std::terminate()
#endif
#endif

#if XMLXX_CXX11
#define XMLXX_MOVE(x) std::move(x)
#else
#define XMLXX_MOVE(x) x
#endif

#endif
