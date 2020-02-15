#include <iostream>
#include <xmlcc/qname.hxx>

namespace xmlcc {

  bool operator<(const qname &lhs, const qname &rhs) XMLCC_NOEXCEPT
  {
    if (lhs.namespace_() < rhs.namespace_()) {
      return true;
    }
    if (lhs.namespace_() == rhs.namespace_()) {
      return lhs.name() < rhs.name();
    }
    return false;
  }

  bool operator==(const qname &lhs, const qname &rhs) XMLCC_NOEXCEPT
  {
    return lhs.namespace_() == rhs.namespace_() && lhs.name() == rhs.name();
  }

  std::ostream &operator<<(std::ostream &os, const qname &q)
  {
    if (q.namespace_().empty())
      return os << q.name();
    return os << q.namespace_() << ':' << q.name();
  }

  qname qname::from_chars(const char *name) { return qname(name); }

} // namespace xmlcc
