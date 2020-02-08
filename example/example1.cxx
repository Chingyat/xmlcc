#include <xmlcc/parser.hxx>

#include <iomanip>
#include <iostream>
#include <sstream>
using namespace xmlcc;

int main()
{
  const char *xml = R"(<object id="12">
  <name>Foo</name>
  <value>Bar</value>
</object>
)";

  std::istringstream is(xml);
  parser p(is, "object.xml",
           (parser::default_feature & ~parser::receive_attributes_map) |
               parser::receive_attributes_event);

  try {
    for (auto e : p) {
      std::cout << e << ' ';
      switch (e) {
      case xmlcc::parser::start_element:
        std::cout << quoted(p.name());
        break;
      case xmlcc::parser::end_element:
        std::cout << quoted(p.name());
        break;
      case xmlcc::parser::start_attribute:
        std::cout << quoted(p.name());
        break;
      case xmlcc::parser::end_attribute:
        std::cout << quoted(p.name());
        break;
      case xmlcc::parser::characters:
        std::cout << quoted(p.value());
        break;
      case xmlcc::parser::eof:
        break;
      }
      std::cout << '\n';
    }
  }
  catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
