#ifndef XMLCC_EXCEPTION_HXX
#define XMLCC_EXCEPTION_HXX

#include <xmlcc/detail/macros.hxx>
#include <xmlcc/detail/export.h>
#include <xmlcc/detail/fwd.hxx>

#include <system_error>

namespace xmlcc {
  class XMLCC_EXPORT parsing : public std::exception {
  public:
    parsing(const std::string &input_name, unsigned long line,
            unsigned long col, const std::string &msg) XMLXX_NOEXCEPT
        : input_name_(input_name),
          line_(line),
          col_(col),
          msg_(msg)
    {
    }

    parsing(parser &par, const std::string &msg) XMLXX_NOEXCEPT;

    const std::string &input_name() const XMLXX_NOEXCEPT { return input_name_; }

    unsigned long line() const XMLXX_NOEXCEPT { return line_; }

    unsigned long column() const XMLXX_NOEXCEPT { return col_; }

    const std::string &message() const XMLXX_NOEXCEPT { return msg_; }

    const char *what() const XMLXX_NOEXCEPT { return what_.c_str(); }

  private:
    std::string input_name_;
    unsigned long line_;
    unsigned long col_;
    std::string msg_;
    std::string what_;
  };

} // namespace xmlcc

#endif // !XMLCC_EXCEPTION
