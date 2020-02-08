#include <sstream>
#include <xmlcc/exception.hxx>
#include <xmlcc/parser.hxx>

namespace xmlcc {
  parsing::parsing(parser &p, const std::string &msg) XMLXX_NOEXCEPT
      : parsing(p.input_name(), p.line(), p.column(), msg)
  {
    std::ostringstream ss;
    ss << input_name_ << ':' << line_ << ':' << col_ << ':' << ' ' << msg_;
    what_ = ss.str();
  }

} // namespace xmlcc
