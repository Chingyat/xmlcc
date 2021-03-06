#include <xmlcc/exception.hxx>
#include <xmlcc/parser.hxx>

#include <iostream>
#include <sstream>

namespace xmlcc {
  parsing::parsing(const std::string &input, unsigned long line,
                   unsigned long column, const std::string &msg)
      : input_name_(input), line_(line), column_(column), msg_(msg)
  {
    std::ostringstream ss;
    ss << input_name_ << ':' << line_ << ':' << column_ << ": " << msg;
    what_ = ss.str();
  }

  parsing::parsing(parser &p, const std::string &msg)
      : input_name_(p.input_name()), line_(p.line()), column_(p.column()),
        msg_(msg)
  {
    std::ostringstream ss;
    ss << input_name_ << ':' << line_ << ':' << column_ << ": " << msg_;
    what_ = ss.str();
  }

  parser::parser(std::istream &is, const std::string &name,
                 feature_type feature)
      : feature_(feature), event_(eof), state_(current_state), ended_(false),
        pis_(&is), is_attr_char_(false), bufsz_(1024), parsing_state_(),
        pqname_(), pvalue_(), input_name_(name)
  {
    p_ = XML_ParserCreateNS(0, ' ');
    // ensure the following code does not throw

    XML_SetUserData(p_, this);
    if (feature_ & receive_elements) {
      XML_SetElementHandler(p_, start_element_, end_element_);
    }

    if (feature_ & receive_characters) {
      XML_SetCharacterDataHandler(p_, characters_);
    }
  }

  parser::~parser() { XML_ParserFree(p_); }

  // suspends the parser
  // called in handler when events are ready
  void parser::suspend(event_type reason) XMLCC_NOEXCEPT
  {
    assert(!parsing_state_.suspended_);
    event_ = reason;
    XML_Status s = XML_StopParser(p_, true);
    if (s == XML_STATUS_ERROR) {
      XML_Error err = XML_GetErrorCode(p_);
      std::cerr << "suspend: " << XML_ErrorString(err) << '\n';
      std::terminate();
    }
    parsing_state_.suspended_ = true;
  }

  // aborts the parser
  // called in handler on error
  void parser::abort(std::exception_ptr const &exc) XMLCC_NOEXCEPT
  {
    XML_Status s = XML_StopParser(p_, false);
    if (s == XML_STATUS_ERROR) {
      XML_Error err = XML_GetErrorCode(p_);
      std::cerr << "suspend: " << XML_ErrorString(err) << '\n';
      std::terminate();
    }

    exc_ = exc;
  }

  parser::event_type parser::next()
  {
    if (state_ == next_state) {
      state_ = current_state;
    }
    else {
      next_();
      prepare_data();
    }

    return event_;
  }

  parser::event_type parser::peek()
  {
    if (state_ == current_state) {
      next_();
      state_ = next_state;
      prepare_data();
    }

    return event_;
  }

  void parser::next_()
  {
    if (feature_ & receive_attributes_event) {
      if (event_ == start_element) {
        if (!attr_.empty()) {
          attr_i_ = 0;
          event_ = start_attribute;
          return;
        }
      }
      else if (event_ == start_attribute) {
        event_ = characters;
        is_attr_char_ = true;
        return;
      }
      else if (event_ == characters && is_attr_char_) {
        is_attr_char_ = false;
        event_ = end_attribute;
        return;
      }
      else if (event_ == end_attribute) {
        if (attr_i_ + 1 < attr_.size()) {
          attr_i_ += 1;
          event_ = start_attribute;
          return;
        }
      }
    }

    if (ended_) {
      assert(event_ == start_element || event_ == end_attribute);
      event_ = end_element;
      return;
    }

    if (event_ == end_element) {
      elem_.pop_back();
    }

    parse();
  }

  // parses the input, returns on events
  void parser::parse()
  {
    parsing_state_.suspended_ = false;

    switch (parsing_state_.case_) {
    case 0:
      while (*pis_) {
        {
          void *b = XML_GetBuffer(p_, bufsz_);
          int len = static_cast<int>(
              pis_->readsome(reinterpret_cast<char *>(b), bufsz_));
          if (len == 0) {
            parsing_state_.case_ = 2;
            break;
          }
          parsing_state_.xml_status_ = XML_ParseBuffer(p_, len, pis_->eof());
        }

        while (parsing_state_.xml_status_ == XML_STATUS_SUSPENDED) {
          parsing_state_.case_ = 1;
          return;
        case 1:
          parsing_state_.xml_status_ = XML_ResumeParser(p_);
        }

        if (parsing_state_.xml_status_ == XML_STATUS_ERROR) {
          if (exc_)
            std::rethrow_exception(exc_);
          XMLCC_THROW(parsing(*this, XML_ErrorString(XML_GetErrorCode(p_))));
        }
      }

    /* fallthrough */
    case 2:
      event_ = eof;
    }
  }

  // prepares the data that can be observed using qname(), value() etc
  void parser::prepare_data()
  {
    if (event_ == eof) {
      pqname_ = 0;
      pvalue_ = 0;
      return;
    }

    assert(!elem_.empty());
    if (event_ == start_element) {
      pqname_ = &elem_.back().name;
      return;
    }

    if (event_ == end_element) {
      pqname_ = &elem_.back().name;
      return;
    }

    if (event_ == characters) {
      if (is_attr_char_) {
        pvalue_ = &attr_[attr_i_].second;
      }
      else {
        pvalue_ = &char_;
      }
      return;
    }

    if (event_ == start_attribute) {
      pqname_ = &attr_[attr_i_].first;
      return;
    }

    if (event_ == end_attribute) {
      pqname_ = &attr_[attr_i_].first;
      return;
    }

    std::terminate();
  }

  const parser::attribute_map_type &parser::attribute_map() const
  {
    return elem_.empty() ? empty_attr_map_ : elem_.back().attr_map;
  }

  const std::string &parser::attribute(const std::string &name) const
  {
    const attribute_value &a = attribute_map().at(name);
    if (!a.handled_) {
      elem_.back().attr_unhandled -= 1;
      a.handled_ = true;
    }
    return a.value_;
  }

  const std::string &parser::attribute(const qname_type &name) const
  {
    const attribute_value &a = attribute_map().at(name);
    if (!a.handled_) {
      elem_.back().attr_unhandled -= 1;
      a.handled_ = true;
    }
    return a.value_;
  }

  void parser::start_element_(void *userdata, const XML_Char *name,
                              const XML_Char **atts)
  {
    parser *const p = reinterpret_cast<parser *>(userdata);

    XMLCC_TRY
    {
      if (!p->elem_.empty()) {
        content_type c = p->elem_.back().content;

        if (c == content::empty || c == content::simple) {
          std::ostringstream ss;
          ss << "unexpected element in " << p->elem_.back().content
             << " content";
          XMLCC_THROW(parsing(*p, ss.str()));
        }
      }
      element_entry el;
      el.name = qname_type::from_chars(name);

      if (p->feature_ & receive_attributes_map) {
        const char **a = atts;
        while (a[0]) {
#ifdef XMLCC_CXX11
          el.attr_map.emplace(qname_type::from_chars(a[0]),
                              attribute_value{a[1], false});
#else
          el.attr_map.insert(attribute_map_type::value_type(
              qname_type::from_chars(a[0]), attribute_value{a[1], false}));
#endif

          a += 2;
        }
        el.attr_unhandled = el.attr_map.size();
      }

      if (p->feature_ & receive_attributes_event) {
        p->attr_.clear();
        const char **a = atts;
        while (a[0]) {
#ifdef XMLCC_CXX11
          p->attr_.emplace_back(qname_type::from_chars(a[0]), a[1]);
#else
          p->attr_.push_back(
              std::make_pair(qname_type::from_chars(a[0]), a[1]));
#endif
          a += 2;
        }
      }

      p->elem_.push_back(XMLCC_MOVE(el));

      p->suspend(start_element);
    }
    XMLCC_CATCH(...) { p->abort(std::current_exception()); }
  }

  void parser::end_element_(void *userdata, const XML_Char *name)
  {
    (void)name;
    parser *const p = reinterpret_cast<parser *>(userdata);
    assert(p->elem_.back().name.name() == name);

    if (p->parsing_state_.suspended_) {
      assert(p->ended_ == false);
      p->ended_ = true;
    }
    else if (p->exc_) {
      return;
    }
    else {
      p->suspend(end_element);
    }
  }

  void parser::characters_(void *userdata, const XML_Char *s, int len)
  {
    parser *const p = reinterpret_cast<parser *>(userdata);
    assert(len >= 0);
    XMLCC_TRY
    {
      if (p->elem_.back().content == content::mixed) {
        p->char_.assign(s, len);
        p->suspend(characters);
      }
      else {
        p->char_.assign(s, len);
        static const char *const ws = " \t\n\r\f\v";
        p->char_.erase(0, p->char_.find_first_not_of(ws));
        p->char_.erase(p->char_.find_last_not_of(ws) + 1);
        if (!p->char_.empty()) {
          if (p->elem_.back().content != content::simple) {
            std::ostringstream ss;
            ss << "non-whitespace characters in " << p->elem_.back().content
               << " content";
            XMLCC_THROW(parsing(*p, ss.str()));
          }
          p->suspend(characters);
        }
      }
    }
    XMLCC_CATCH(...) { p->abort(std::current_exception()); }
  }

  unsigned long parser::line() const XMLCC_NOEXCEPT
  {
    return XML_GetCurrentLineNumber(p_);
  }

  unsigned long parser::column() const XMLCC_NOEXCEPT
  {
    return XML_GetCurrentColumnNumber(p_);
  }

  std::ostream &operator<<(std::ostream &os, parser::event_type e)
  {
    return os << [](parser::event_type e) -> const char * {
      switch (e) {
      case xmlcc::parser::start_element:
        return "start element";

      case xmlcc::parser::end_element:
        return "end element";

      case xmlcc::parser::start_attribute:
        return "start attribute";

      case xmlcc::parser::end_attribute:
        return "end attribute";

      case xmlcc::parser::characters:
        return "characters";

      case xmlcc::parser::eof:
        return "eof";

      default:
        std::terminate();
      }
    }(e);
  }

} // namespace xmlcc
