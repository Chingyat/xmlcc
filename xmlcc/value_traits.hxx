#ifndef XMLCC_VALUE_TRAITS
#define XMLCC_VALUE_TRAITS

#include <xmlcc/detail/fwd.hxx>

#include <sstream>
#include <string>

namespace xmlcc {
  template <typename T> struct value_traits {
    static T parse(const std::string &s, parser const &)
    {
      T r;
      std::istringstream(s) >> r;
      return r;
    }

    // template <typename Serializer>
    // static std::string serialize(const T &v, Serializer const &)
    // {
    //   std::ostringstream ss;
    //   ss << v;
    //   return ss.str();
    // }
  };
} // namespace xmlcc

#endif // XMLCC_VALUE_TRAITS
