#ifndef XMLCC_DETAIL_CONTENT_HXX
#define XMLCC_DETAIL_CONTENT_HXX

#include <xmlcc/detail/export.h>
#include <xmlcc/detail/macros.hxx>

#include <iosfwd>

namespace xmlcc {
#ifdef XMLXX_CXX11
  enum class content : unsigned char {
    empty,
    simple,
    complex,
    mixed,
  };

  XMLCC_EXPORT std::ostream &operator<<(std::ostream &os, const content &cnt);

#else
  struct content;

  std::ostream &operator<<(std::ostream &os, const content &cnt);

  struct XMLCC_NO_EXPORT content {
    friend std::ostream &operator<<(std::ostream &os, const content &cnt);

    enum _enum { empty, simple, complex, mixed };

    content(_enum v) : v_(v) {}

    bool operator==(const content &other) const XMLXX_NOEXCEPT
    {
      return v_ == other.v_;
    }

    bool operator!=(const content &other) const XMLXX_NOEXCEPT
    {
      return !(*this == other);
    }

  private:
    _enum v_;
  };
#endif
} // namespace xmlcc

#endif // XMLCC_DETAIL_CONTENT_HXX
