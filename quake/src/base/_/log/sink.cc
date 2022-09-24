#include "sink.h"

#include <base/log.h>

#ifdef OS_POSIX
#include "log-file-posix.h"
#endif  // OS_POSIX

namespace base {
namespace log {
namespace _ {
namespace {

class ConsoleSink : public Sink {
 public:
  static std::shared_ptr<Sink> Create(const Parser::Attributes&);

  // Write the data with specified Severity into the |console|
  void Write(Severity, const std::string&, bool) override;
  void BeforeFork() override;

  std::string GetName() const override { return "console"; }
};

std::shared_ptr<Sink> ConsoleSink::Create(const Parser::Attributes& a) {
  UNUSE(a);
  return std::make_shared<ConsoleSink>();
}

void ConsoleSink::Write(Severity severity, const std::string& msg, bool flush) {
  // Notes(dpeng): The following code is actually not transactional or atomic,
  //   the flush and the written of data out
  switch (severity) {
    case kInfo:
    case kRaw:
      std::cout << msg;
      if (flush) {
        std::cout.flush();
      }
      break;
    case kWarning:
    case kError:
    case kFatal:
    case kBug:
    case kTrace:
      std::cerr << msg;
      std::cout.flush();
      break;
    default:
      break;
  }
}

void ConsoleSink::BeforeFork() {
  // Flush the internal buffer out before fork since after the fork the data
  // which left inside of the buffer may be duplicated into the parent and
  // child process which may cause suprise result
  std::cout.flush();
  std::cerr.flush();
}

#ifdef OS_POSIX

class PosixFileSink : public Sink {
 public:
  static std::shared_ptr<Sink> Create(const Parser::Attributes&);

 public:
  PosixFileSink(const std::string& fn) : f_(fn) {}

  bool Init() { return f_.Init(); }
  void Write(Severity, const std::string&, bool) override;
  void BeforeFork() override { f_.Flush(); }
  void Rotate() override { f_.Rotate(); }
  void Flush() override { f_.Flush(); }

  std::string GetName() const override {
    return Fmt("posix_file(path='%s')", f_.filename().c_str());
  }

 private:
  LogFilePosix f_;
};

void PosixFileSink::Write(Severity severity, const std::string& msg, bool f) {
  f_.Write(msg, f);
}

std::shared_ptr<Sink> PosixFileSink::Create(const Parser::Attributes& a) {
  static const char* kDefault = "./error.log";
  std::string filename{kDefault};
  if (auto v = Parser::Get<std::string>(a, "path"); v) {
    filename = v.value();

    if (std::filesystem::path p{filename}; p.has_parent_path()) {
      // Notes(dpeng): Here we try to mkdir if needed. The user specified path
      //   is allowed to have multiple layers of directory, and each one may or
      //   may not exist. This call ensure the dir is in place or error happened
      std::error_code ec;
      std::filesystem::create_directories(p.parent_path(), ec);
      if (ec) {
        UWARNING("posix_file sink fail: %s", ec.message().c_str());
        return std::shared_ptr<Sink>{};
      }
    }
  }

  if (auto r = std::make_shared<PosixFileSink>(filename); r->Init()) {
    return r;
  }

  UWARNING("posix_file sink fail: %s", std::strerror(errno));
  return std::shared_ptr<Sink>{};
}

#endif  // OS_POSIX

}  // namespace

std::shared_ptr<Sink> Sink::Create(const std::string& name,
                                   const Parser::Attributes& attr) {
#ifdef OS_POSIX
  if (name == "posix_file") {
    return PosixFileSink::Create(attr);
  }
#endif  // OS_POSIX

  if (name == "console") {
    return ConsoleSink::Create(attr);
  }
  return std::shared_ptr<Sink>{};
}

std::shared_ptr<Sink> Sink::CreateDefault() {
  return ConsoleSink::Create(Parser::Attributes{});
}

}  // namespace _
}  // namespace log
}  // namespace base
