#include "logger.h"

namespace base {
namespace log {
namespace _ {
namespace {

const char* kBaseLogConfig = "BASE_LOG";
const char* kBaseLogVerboseLevel = "BASE_VLOG";

struct PerThread {
  std::uint32_t verbose;
  inline PerThread();
};

// The per thread data used to dynamically modify behavior of LOG usage.
thread_local PerThread kData;

// This function tries to get the config DSL during setup. It basically just use
// environment variable BASE_LOG to overwrite the external CONFIG_base_log
std::string GetConfigDSL() {
  if (auto p = GetEnv<std::string>(kBaseLogConfig); p) {
    return p.value();
  } else {
    return CONFIG_base_log;
  }
}

}  // namespace

// sink(filter=[... list of filters ...])
void SinkList::Init(const Parser::Attributes& a) {
  if (auto v = Parser::Get<std::shared_ptr<Parser::List>>(a, "filter"); v) {
    for (auto& entry : *v.value()) {
      if (auto vv = Parser::Get<std::string>(entry); vv) {
        std::string name;
        Parser::Attributes attr;

        if (auto x = Parser(vv.value()).ParseItem(&name, &attr); x.empty()) {
          if (auto s = Sink::Create(name, attr); s) {
            sink_list_.push_back(std::move(s));
          }
        }
      }
    }
  }
}

void SinkList::InitDefault() { sink_list_.push_back(Sink::CreateDefault()); }

void SinkList::Rotate() {
  std::lock_guard<std::mutex> g{mutex_};
  for (auto& s : sink_list_) {
    s->Rotate();
  }
}

void SinkList::Flush() {
  std::lock_guard<std::mutex> g{mutex_};
  for (auto& s : sink_list_) {
    s->Flush();
  }
}

void SinkList::Write(Severity s, std::string&& data, bool v) {
  std::lock_guard<std::mutex> g{mutex_};
  for (auto& entry : sink_list_) {
    entry->Write(s, data, v);
  }
}

void SinkList::BeforeFork() {
  std::lock_guard<std::mutex> g{mutex_};
  for (auto& s : sink_list_) {
    s->BeforeFork();
  }
}

void SinkList::AfterFork() {
  std::lock_guard<std::mutex> g{mutex_};
  for (auto& s : sink_list_) {
    s->AfterFork();
  }
}
void Logger::Record(const char* file, int line, const char* tag,
                    Severity severity, std::string&& msg) {
  auto& e = entry[severity];
  std::string data;

  if (severity == kRaw) {
    data = std::move(msg);
  } else {
    data = e.formatter->Format(file, line, tag, severity, std::move(msg),
                               e.stacktrace);
  }

  // Always append a linebreak after each formatter log line
  data.push_back('\n');

  // Invoke user's sink function if provided.
  if (user_sink) {
    user_sink(severity, data, e.flush, e.terminate);
  }

  sink_list.Write(severity, std::move(data), e.flush);
  if (e.terminate) {
    Terminate();
  }
}

void Logger::Terminate() { DieNow(); }

void Logger::Populate(const Parser::Attributes& a, LogEntry* o, int severity) {
  static const LogEntry::Option kInitTemplate[] = {
      {false, false, false}, {false, true, false}, {false, true, false},
      {true, true, true},    {true, true, false},  {false, true, false},
      {false, false, false}};

  o->Set(kInitTemplate[severity]);

  if (auto v = Parser::Get<bool>(a, "stacktrace"); v) {
    o->stacktrace = v.value();
  }
  if (auto v = Parser::Get<bool>(a, "flush"); v) {
    o->flush = v.value();
  }
  if (auto v = Parser::Get<bool>(a, "terminate"); v) {
    o->terminate = v.value();
  }

  {
    if (auto v = Parser::Get<std::string>(a, "formatter"); v) {
      Parser::Attributes attr;
      std::string name;
      if (auto x = Parser(v.value()).ParseItem(&name, &attr); x.empty()) {
        if (auto f = Formatter::Create(name, attr); f) {
          o->formatter = std::move(f);
        }
      }
    }
    if (!o->formatter) {
      o->formatter = Formatter::CreateDefault();
    }
  }
}

int Logger::MapSeverityFromName(const std::string& n) {
#define __(A, B, C) \
  if (n == (C)) return A;
  BASE_LOG_SEVERITY_LIST(__)
#undef __  // __
  return -1;
}

// The main initialization part of the LOG
Logger::Logger() {
  auto config = GetConfigDSL();
  Parser::Table t;

  // Parse the config into Parser::Table
  if (auto r = Parser(config).Parse(&t); !r.empty()) {
    UFATAL("Log initialization failed: invalid config): %s", r.c_str());
    return;
  }

  // Configure each entry
  {
    for (const auto& kv : t) {
      if (auto severity = MapSeverityFromName(kv.first); severity != -1) {
        Populate(kv.second, entry + severity, severity);
      }
    }

    // We may have entry that doesn't show up in configuration DSL, they still
    // need to be configured.
    int severity = 0;
    for (auto& e : entry) {
      // The populate will guarantee that formatter is not null, so if formatter
      // is null here it means no entry show up in configuration DSL, we need to
      // configure it in default way
      if (!e.formatter) {
        Populate(Parser::Attributes{}, &e, severity);
      }
      ++severity;
    }
  }

  if (auto sink = t.find("sink"); sink != t.end()) {
    sink_list.Init(sink->second);
  }
  if (sink_list.IsEmpty()) {
    sink_list.InitDefault();
  }
  if (auto v = GetEnv<std::uint32_t>(kBaseLogVerboseLevel); v) {
    verbose = v.value();
  }
}

Logger* GetLogger() {
  // Log itself should be leaked since we really don't know the exact point
  // for destructing it, atexit still can reference log somehow. For now just
  // leak it.
  static base::NoDestructor<Logger> kLogger;
  return kLogger.get();
}

void SetVerboseLevelCurrentThread(std::uint32_t l) { _::kData.verbose = l; }

std::uint32_t GetVerboseLevelCurrentThread() { return _::kData.verbose; }

namespace {

PerThread::PerThread() : verbose(GetLogger()->verbose) {}

}  // namespace

}  // namespace _
}  // namespace log
}  // namespace base
