#ifndef XMLCC_SERIALIZER_HXX
#define XMLCC_SERIALIZER_HXX


#include <xmlcc/detail/export.h>
#include <xmlcc/detail/fwd.hxx>
#include <xmlcc/detail/macros.hxx>
#include <xmlcc/content.hxx>

#include <xmlcc/detail/genx/genx.h>

#include <string>
#include <iosfwd>
#include <vector>

namespace xmlcc {
  class XMLCC_EXPORT serializer : private std::allocator<char> {
  public:
#ifdef XMLXX_CXX11
    typedef enum content content_type;
#else
    typedef struct content content_type;
#endif

    typedef std::allocator<char> allocator_type;

    serializer(std::ostream &os, const std::string &output_name);

    ~serializer();

    allocator_type get_allocator() const { return static_cast<const allocator_type &>(*this); }

  private:
    std::ostream *pos_;
    std::string output_name_;
    genxWriter w_;
    genxSender sender_;

    static void *alloc_(void *userData, int size);
    static void dealloc_(void *userData, void *data);

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

    void element(const std::string &name, const std::string &value)
    {
      start_element(name);
      characters(value);
      end_element();
    }

    void attribute(const std::string &name, const std::string &value)
    {
      start_attribute(name);
      characters(value);
      end_attribute();
    }
  };
} // namespace xmlcc

#endif // XMLCC_SERIALIZER_HXX
