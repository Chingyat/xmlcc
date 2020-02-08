#include <sstream>
#include <xmlcc/exception.hxx>
#include <xmlcc/parser.hxx>

namespace xmlcc {
  parsing::parsing(parser &p, const std::string &msg) XMLXX_NOEXCEPT
      : input_name_(p.input_name()),
        line_(p.line()),
        col_(p.column()),
        msg_(msg)
  {
    std::ostringstream ss;
    ss << input_name_ << ':' << line_ << ':' << col_ << ':' << ' ' << msg_;
    what_ = ss.str();
  }

} // namespace xmlcc
