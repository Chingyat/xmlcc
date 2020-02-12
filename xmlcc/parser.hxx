#ifndef XMLCC_PARSER_HXX
#define XMLCC_PARSER_HXX

#include <expat.h>

#include <cassert>
#include <map>
#include <vector>

#include <xmlcc/content.hxx>
#include <xmlcc/detail/export.h>
#include <xmlcc/detail/macros.hxx>
#include <xmlcc/qname.hxx>
#include <xmlcc/exception.hxx>

namespace xmlcc {

  class XMLCC_EXPORT parsing : public exception {
  public:
    parsing(const std::string &input_name, unsigned long line,
            unsigned long col, const std::string &msg) XMLXX_NOEXCEPT
        : input_name_(input_name),
          line_(line),
          col_(col),
          msg_(msg)
    {
    }

    parsing(parser &par, const std::string &msg) XMLXX_NOEXCEPT;

    const std::string &input_name() const XMLXX_NOEXCEPT { return input_name_; }

    unsigned long line() const XMLXX_NOEXCEPT { return line_; }

    unsigned long column() const XMLXX_NOEXCEPT { return col_; }

    const std::string &message() const XMLXX_NOEXCEPT { return msg_; }

    const char *what() const XMLXX_NOEXCEPT { return what_.c_str(); }

  private:
    std::string input_name_;
    unsigned long line_;
    unsigned long col_;
    std::string msg_;
    std::string what_;
  };

  class XMLCC_EXPORT parser {
  public:
    typedef unsigned feature_type;
    enum {
      receive_elements = 0x01,
      receive_attributes_map = 0x02,
      receive_attributes_event = 0x04,
      receive_characters = 0x08,

      default_feature =
          receive_elements | receive_attributes_map | receive_characters,
    };

    enum event_type {
      start_element,
      end_element,
      start_attribute,
      end_attribute,
      characters,
      eof,
    };

#ifdef XMLXX_CXX11
    typedef enum content content_type;
#else
    typedef struct content content_type;
#endif

    // creates the parser
    parser(std::istream &is, const std::string &name,
           feature_type feature = default_feature);

    ~parser();

    // sets the content type of the current element
    void content(content_type c) { elem_.back().content = c; }

    typedef class qname qname_type;

  private:
    feature_type feature_;

    // the event returned by the last next() or peek() called
    event_type event_;

    enum {
      next_state,    // indicates that the event has been peeked
                     // e.g., peek() has just been called
      current_state, // indicates that the event is handled
                     // e.g., next() has just been called
    } state_;

    bool suspended_;

    // when handling start element, indicates whether
    // the end element handler is already invoked
    bool ended_;

    std::istream *pis_;

    XML_Parser p_;

    std::vector<std::pair<qname_type, std::string>> typedef attributes;

    // attribute events of current element
    attributes attr_;

    // index of the attribute event being handled
    attributes::size_type attr_i_;

    // to indicate whether the characters are attribute value
    bool is_attr_char_;

    // character data
    std::string char_;

    struct attribute_value {
      std::string value_;
      mutable bool handled_;
    };
    typedef std::map<qname_type, attribute_value> attribute_map_type;

    // empty attribute map to be returned when there are no element
    // in the element vector
    const attribute_map_type empty_attr_map_;
    struct element_entry {
      qname_type name;
      content_type content;
      attribute_map_type attr_map;
      mutable attribute_map_type::size_type attr_unhandled;

      element_entry() XMLXX_NOEXCEPT : content(content_type::mixed),
                                       attr_unhandled(false)
      {
      }
    };
    std::vector<element_entry> typedef elements;

    elements elem_;

    void suspend(event_type reason) XMLXX_NOEXCEPT;

    void abort(const std::exception_ptr &reason) XMLXX_NOEXCEPT;

  public:
    event_type event() const { return event_; }

    event_type next();

    void next_expect(event_type e)
    {
      if (next() != e) {
        std::terminate();
      }
    }

    void next_expect(event_type e, const std::string &s)
    {
      next_expect(e);
      if (name() != s) {
        std::terminate();
      }
    }

    void next_expect(event_type e, const qname_type &n)
    {
      next_expect(e);
      using namespace std::rel_ops;
      if (qname() != n) {
        std::terminate();
      }
    }

    void next_expect(event_type e, const std::string &s, content_type c)
    {
      next_expect(e, s);
      content(c);
    }

    void next_expect(event_type e, const qname_type &s, content_type c)
    {
      next_expect(e, s);
      content(c);
    }

    // peek the next event
    event_type peek();

  private:
    void next_();

    // buffer size
    int bufsz_;

    XML_Status xml_status_;
    int case_;
    void parse();

    static void XMLCALL start_element_(void *userdata, const XML_Char *name,
                                       const XML_Char **atts);

    static void XMLCALL end_element_(void *userdata, const XML_Char *name);

    static void XMLCALL characters_(void *userdata, const XML_Char *s, int len);

  public:
    const qname_type &qname() const
    {
      return pqname_ ? *pqname_ : empty_qname_;
    }
    const std::string &prefix() const { return qname().prefix(); }
    const std::string &namespace_() const { return qname().namespace_(); }
    const std::string &name() const { return qname().name(); }
    const std::string &value() const
    {
      return pvalue_ ? *pvalue_ : empty_qname_.name();
    }

  private:
    const qname_type empty_qname_;

    const qname_type *pqname_;
    const std::string *pvalue_;

  public:
    const attribute_map_type &attribute_map() const;

    const std::string &attribute(const std::string &name) const;

    const std::string &attribute(const qname_type &name) const;

    bool attribute_present(const std::string &name) const XMLXX_NOEXCEPT
    {
      return 0 != attribute_map().count(name);
    }

    bool attribute_present(const qname_type &name) const XMLXX_NOEXCEPT
    {
      return 0 != attribute_map().count(name);
    }

    std::string attribute(const std::string &name,
                          const std::string &default_value) const
    {
      if (attribute_present(name))
        return attribute(name);
      return default_value;
    }

    std::string attribute(const qname_type &name,
                          const std::string &default_value) const
    {
      if (attribute_present(name))
        return attribute(name);
      return default_value;
    }

    std::string element()
    {
      assert(event_ == start_element);
      content(content::simple);
      next_expect(characters);
      std::string r = value();
      next_expect(end_element);
      return r;
    }

    std::string element(const std::string &name)
    {
      next_expect(start_element, name);
      return element();
    }

    std::string element(const qname_type &name)
    {
      next_expect(start_element, name);
      return element();
    }

    std::string element(const std::string &name,
                        const std::string &default_value)
    {
      // FIXME: semantic unambiguous

      if (peek() == start_element)
        return element(name);
      return default_value;
    }

    std::string element(const qname_type &name,
                        const std::string &default_value)
    {
      // FIXME: semantic unambiguous

      if (peek() == start_element)
        return element(name);
      return default_value;
    }

  private:
    void prepare_data();

  private:
    std::exception_ptr exc_;

  public:
    unsigned long line() const XMLXX_NOEXCEPT;

    unsigned long column() const XMLXX_NOEXCEPT;

    const std::string &input_name() const XMLXX_NOEXCEPT { return input_name_; }

  private:
    std::string input_name_;

  public:
    template <typename T> T attribute(const std::string &name) const;

#ifndef XMLXX_CXX11
    template <typename T>
    T attribute(const std::string &name, const T &default_value) const;
#else
    template <typename T, typename U>
    T attribute(const std::string &name, U &&default_value) const;
#endif

    template <typename T> T element();

    template <typename T> T element(const std::string &name);

#ifndef XMLXX_CXX11
    template <typename T>
    T element(const std::string &name, const T &default_value);
#else
    template <typename T, typename U>
    T element(const std::string &name, U &&default_value);
#endif

    class iterator {
    public:
      typedef std::input_iterator_tag iterator_category;
      typedef event_type value_type;
      typedef const event_type &reference;
      typedef const event_type *pointer;

      iterator() XMLXX_NOEXCEPT : p_(0), e_(eof) {}

      iterator(parser *p, event_type e = eof)
          : p_(p), e_(e == eof ? p->next() : e)
      {
      }

      bool operator==(iterator const &other) const XMLXX_NOEXCEPT
      {
        return e_ == other.e_;
      }

      bool operator!=(iterator const &other) const XMLXX_NOEXCEPT
      {
        return !(*this == other);
      }

      reference operator*() const XMLXX_NOEXCEPT { return e_; }

      iterator &operator++()
      {
        e_ = p_->next();
        return *this;
      }

      iterator operator++(int)
      {
        iterator r = *this;
        ++*this;
        return r;
      }

    private:
      parser *p_;
      event_type e_;
    };

    iterator begin() { return iterator(this); }
    iterator end() { return iterator(); }
  };

  XMLCC_EXPORT std::ostream &operator<<(std::ostream &os, parser::event_type e);

} // namespace xmlcc

#include <xmlcc/parser.txx>

#endif
