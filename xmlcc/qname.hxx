#ifndef XMLCC_QNAME_HXX
#define XMLCC_QNAME_HXX

#include <string>

#include <xmlcc/detail/export.hxx>
#include <xmlcc/detail/macros.hxx>

namespace xmlcc {

  class XMLCC_EXPORT qname {
  public:
    qname() XMLCC_NOEXCEPT {}
    qname(const std::string &name) : name_(name) {}
    qname(const std::string &ns, const std::string &name) : ns_(ns), name_(name)
    {
    }
    qname(const std::string &prefix, const std::string &ns,
          const std::string &name)
        : prefix_(prefix), ns_(ns), name_(name)
    {
    }

    static qname from_chars(const char *name);

    const std::string &prefix() const { return prefix_; }
    const std::string &namespace_() const { return ns_; }
    const std::string &name() const { return name_; }

    void swap(qname &other) XMLCC_NOEXCEPT
    {
      using std::swap;
      swap(prefix_, other.prefix_);
      swap(ns_, other.ns_);
      swap(name_, other.name_);
    }

  private:
    std::string prefix_;
    std::string ns_;
    std::string name_;
  };

  XMLCC_EXPORT bool operator<(const qname &lhs,
                              const qname &rhs) XMLCC_NOEXCEPT;

  XMLCC_EXPORT bool operator==(const qname &lhs,
                               const qname &rhs) XMLCC_NOEXCEPT;

  XMLCC_EXPORT std::ostream &operator<<(std::ostream &os, const qname &name);
} // namespace xmlcc

#endif
