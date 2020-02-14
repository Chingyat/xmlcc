#include <xmlcc/serializer.hxx>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace xmlcc {

  namespace {
    constUtf8 toConstUtf8(const std::string &s)
    {
      return reinterpret_cast<constUtf8>(s.c_str());
    }
  } // namespace

  serializer::serializer(std::ostream &os, const std::string &output_name)
      : pos_(&os), output_name_(output_name), attr_()
  {
#ifdef XMLXX_SERIALIZER_USE_ALLOCATOR
    w_ = genxNew(alloc_, dealloc_, this); // FIXME: use allocator
#else
    w_ = genxNew(0, 0, 0);
#endif
    genxSetUserData(w_, this);

    sender_.send = send_;
    sender_.sendBounded = sendBounded_;
    sender_.flush = flush_;

    XMLXX_TRY
    {
      genxStatus s = genxStartDocSender(w_, &sender_);
      if (s) {
        XMLXX_THROW s;
      }
    }
    XMLXX_CATCH(...)
    {
      genxDispose(w_);

      XMLXX_RETHROW;
    }
  }

  serializer::~serializer()
  {
    //   ::genxEndDocument(w_);
    ::genxDispose(w_);
  }

  void serializer::eof()
  {
    genxStatus s = genxEndDocument(w_);
    check_status(s);
  }

#ifdef XMLXX_SERIALIZER_USE_ALLOCATOR
  void *serializer::alloc_(void *userData, int size)
  {
    return reinterpret_cast<serializer *>(userData)->allocate(size);
  }

  void serializer::dealloc_(void *userData, void *data)
  {
    reinterpret_cast<serializer *>(userData)->deallocate(
        reinterpret_cast<char *>(data), 0);
  }
#endif

  genxStatus serializer::send_(void *userData, constUtf8 s)
  {
    reinterpret_cast<const serializer *>(userData)->pos_->write(
        reinterpret_cast<const char *>(s),
        std::strlen(reinterpret_cast<const char *>(s)));
    return GENX_SUCCESS;
  }

  genxStatus serializer::sendBounded_(void *userData, constUtf8 start,
                                      constUtf8 end)
  {
    reinterpret_cast<const serializer *>(userData)->pos_->write(
        reinterpret_cast<const char *>(start), end - start);
    return GENX_SUCCESS;
  }

  genxStatus serializer::flush_(void *userData)
  {
    reinterpret_cast<const serializer *>(userData)->pos_->flush();
    return GENX_SUCCESS;
  }

  void serializer::check_status(genxStatus s) const
  {
    if (s != 0) {
      XMLXX_THROW std::runtime_error(genxGetErrorMessage(w_, s));
    }
  }

  void serializer::start_element(const std::string &name)
  {
    genxStatus s = genxStartElementLiteral(w_, 0, toConstUtf8(name));
    check_status(s);
  }

  void serializer::end_element()
  {
    genxStatus s = genxEndElement(w_);
    check_status(s);
  }

  void serializer::start_attribute(const std::string &name)
  {
    genxStatus s;
    attr_ = genxDeclareAttribute(w_, 0, toConstUtf8(name), &s);
    check_status(s);
  }

  void serializer::end_attribute()
  {
    genxStatus s = genxAddAttribute(attr_, toConstUtf8(value_));
    check_status(s);
    attr_ = 0;
  }

  void serializer::characters(const std::string &value)
  {
    if (attr_) {
      value_ = value;
    }
    else {
      genxStatus s = genxAddText(w_, toConstUtf8(value));
      check_status(s);
    }
  }

} // namespace xmlcc
