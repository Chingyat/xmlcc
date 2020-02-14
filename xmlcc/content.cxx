#include <iostream>
#include <xmlcc/content.hxx>

namespace xmlcc {
  std::ostream &operator<<(std::ostream &os, const content &c)
  {
#ifdef XMLXX_CXX11
    switch (c) {
#else
    switch (c.v_) {
#endif
    case content::empty:
      return os << "empty";

    case content::simple:
      return os << "simple";

    case content::complex:
      return os << "complex";

    case content::mixed:
      return os << "mixed";
    }
  }

} // namespace xmlcc