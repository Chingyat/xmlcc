#ifndef XMLCC_EXCEPTION_HXX
#define XMLCC_EXCEPTION_HXX

#include <xmlcc/detail/export.hxx>
#include <xmlcc/detail/fwd.hxx>
#include <xmlcc/detail/macros.hxx>

#include <system_error>

namespace xmlcc {

  class XMLCC_EXPORT exception : public std::exception {
  public:
    const char *what() const XMLCC_NOEXCEPT { return "xmlcc"; }
  };

} // namespace xmlcc

#endif // !XMLCC_EXCEPTION
