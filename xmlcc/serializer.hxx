#ifndef XMLCC_SERIALIZER_HXX
#define XMLCC_SERIALIZER_HXX

#include <xmlcc/content.hxx>
#include <xmlcc/detail/export.hxx>
#include <xmlcc/detail/fwd.hxx>
#include <xmlcc/detail/macros.hxx>
#include <xmlcc/value_traits.hxx>

#ifndef XMLCC_USE_EXTERNAL_GENX
#include <xmlcc/detail/genx/genx.h>
#else
#include <genx.h>
#endif // !XMLCC_USE_EXTERNAL_GENX

#include <iosfwd>
#include <string>
#include <vector>

namespace xmlcc {
  class XMLCC_EXPORT serializer
#ifdef XMLCC_SERIALIZER_USE_ALLOCATOR
      : private std::allocator<char>
#endif
  {
  public:
#ifdef XMLCC_CXX11
    typedef enum content content_type;
#else
    typedef struct content content_type;
#endif

#ifdef XMLCC_SERIALIZER_USE_ALLOCATOR
    typedef std::allocator<char> allocator_type;
#endif

    serializer(std::ostream &os, const std::string &output_name);

    ~serializer();

#ifdef XMLCC_SERIALIZER_USE_ALLOCATOR
    allocator_type get_allocator() const
    {
      return static_cast<const allocator_type &>(*this);
    }
#endif

  private:
    std::ostream *pos_;
    std::string output_name_;
    genxWriter w_;
    genxSender sender_;

#ifdef XMLCC_SERIALIZER_USE_ALLOCATOR
    static void *alloc_(void *userData, int size);
    static void dealloc_(void *userData, void *data);
#endif

    static genxStatus send_(void *userData, constUtf8 s);
    static genxStatus sendBounded_(void *userData, constUtf8 start,
                                   constUtf8 end);
    static genxStatus flush_(void *userData);

    genxAttribute attr_;
    std::string value_;

    void check_status(genxStatus s) const;

  public:
    void start_element(const std::string &name);
    void end_element();

    void start_attribute(const std::string &name);
    void end_attribute();

    void characters(const std::string &value);

    void eof();

#ifndef XMLCC_CXX11
    void element(const char *value)
    {
      element(static_cast<std::string>(value));
    }
#endif
    void element(const std::string &value)
    {
      characters(value);
      end_element();
    }

#ifndef XMLCC_CXX11
    void element(const std::string &name, const char *value)
    {
      element(name, static_cast<std::string>(value));
    }
#endif
    void element(const std::string &name, const std::string &value)
    {
      start_element(name);
      element(value);
    }

#ifndef XMLCC_CXX11
    void attribute(const std::string &name, const char *value)
    {
      attribute(name, static_cast<std::string>(value));
    }
#endif

    void attribute(const std::string &name, const std::string &value)
    {
      start_attribute(name);
      characters(value);
      end_attribute();
    }

#ifdef XMLCC_CXX11
    template <
        typename T,
        typename std::enable_if<
            std::is_convertible<const T &, std::string>::value, int>::type = 0>
#else
    template <typename T>
#endif
    void element(const T &value)
    {
      XMLCC_CONSTEXPR const value_traits<T> traits = value_traits<T>();
      element(traits.serialize(value));
    }

#ifdef XMLCC_CXX11
    template <
        typename T,
        typename std::enable_if<
            std::is_convertible<const T &, std::string>::value, int>::type = 0>
#else
    template <typename T>
#endif
    void element(const std::string &name, const T &value)
    {
      XMLCC_CONSTEXPR const value_traits<T> traits = value_traits<T>();
      element(name, traits.serialize(value));
    }

#ifdef XMLCC_CXX11
    template <
        typename T,
        typename std::enable_if<
            std::is_convertible<const T &, std::string>::value, int>::type = 0>
#else
    template <typename T>
#endif
    void attribute(const std::string &name, const T &value)
    {
      XMLCC_CONSTEXPR const value_traits<T> traits = value_traits<T>();
      attribute(name, traits.serialize(value));
    }
  };
} // namespace xmlcc

#endif // XMLCC_SERIALIZER_HXX
