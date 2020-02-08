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
  parser p(is, "object.xml");

  try {
    p.next_expect(parser::start_element, "object");
    p.content(content::complex);
    std::cout << "id: " << p.attribute<int>("id") << "\n";
    std::cout << "name: " << std::quoted(p.element("name")) << "\n";
    std::cerr << "value: " << std::quoted(p.element("value")) << "\n";
    p.next_expect(parser::end_element);
    p.next_expect(parser::eof);
  }
  catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
