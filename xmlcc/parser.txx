#ifndef XMLCC_PARSER_TPP
#define XMLCC_PARSER_TPP

#include <xmlcc/parser.hxx>

namespace xmlcc {

  template <typename T> T parser::attribute(const std::string &name) const
  {
    const value_traits<T> traits;
    return traits.parse(attribute(name), *this);
  }

#if XMLXX_CXX11
  template <typename T, typename U>
  T parser::attribute(const std::string &name, U &&default_value) const
  {
    if (attribute_present(name))
      return attribute<T>(name);
    return std::forward<T>(default_value);
  }
#else
  template <typename T>
  T parser::attribute(const std::string &name, const T &default_value) const
  {
    if (attribute_present(name))
      return attribute<T>(name);
    return default_value;
  }
#endif

  template <typename T> T parser::element()
  {
    const value_traits<T> traits;
    return traits.parse(element(), const_cast<parser const &>(*this));
  }

  template <typename T> T parser::element(const std::string &name)
  {
    const value_traits<T> traits;
    return traits.parse(element(name), const_cast<parser const &>(*this));
  }

#if 0
#if !XMLXX_CXX11
  template <typename T>
  T parser:: element(const std::string &name, const T &default_value) {
    const value_traits<T> traits;
    
  }
#else
  template <typename T, typename U>
  T parser:: element(const std::string &name, U &&default_value);
#endif
#endif
} // namespace xmlcc

#endif
