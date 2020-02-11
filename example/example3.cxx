#include <xmlcc/serializer.hxx>

#include <iostream>

using namespace xmlcc;

int main()
{
  serializer s(std::cout, "object.xml");

  try {
    s.start_element("object");
    s.attribute("id", "123");
    s.element("name", "Foo");
    s.element("value", "Bar");
    s.end_element();
    s.eof();
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
