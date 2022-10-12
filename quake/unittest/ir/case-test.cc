#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <template.h>
#include <quake.h>

#include <ir/parser.h>
#include <ir/graph.h>
#include <ir/builder.h>

#include "qjs-bytecode-test-util.h"
#include "ir-equal-util.h"

// Testing external case file

using namespace base;

namespace quake {
namespace ir {
namespace _ {

// Verify the cases specified by the path and return the true/false as
// indication of whether the test failed or not
bool TestCase(const std::string_view& path) {
  if (auto file = ParseTemplateFile(path); !file) {
    auto e = file.PassError();
    LOG(ERROR) << "parsing " << path << " failed: " << e.GetDisplay();
    return false;
  } else {
    // finding out the js section and hir section
    auto temp = file.PassResult();

    auto maybe_js = temp.GetSection("js");
    if (!maybe_js) {
      LOG(ERROR) << "file: " << path << " does not have javascript section";
      return false;
    }

    auto maybe_ir = temp.GetSection("ir");
    if (!maybe_ir) {
      LOG(ERROR) << "file: " << path << " does not have ir section";
      return false;
    }

    Quake q{nullptr};
    Graph qjs_graph{&q};
    Builder qjs_builder{&q, &qjs_graph};

    // Parsing JavaScript and translate the JavaScript into the IR graph
    {
      QJSFunctionBytecode bc;

      bc.SetUp();
      bc.Compile(*maybe_js);

      auto has_start = false;
      bc.ForeachBytecodeInNaturalLoop(
          [&](InterpInfo* info) {
            if (!has_start) {
              has_start = true;
              auto r = qjs_builder.Setup(info);
              if (!r) {
                LOG(FATAL) << r.message();
              }
            }
            if (auto r = qjs_builder.Trace(info); !r) {
              LOG(FATAL) << r.message();
            }
          },
          [&](InterpInfo* info) { qjs_builder.Stop(info); });
    }

    // Parsing the IR graph
    Graph ir_graph{&q};
    {
      Parser p{&q};
      if (auto e = p.Parse(*maybe_ir, &ir_graph); e) {
        LOG(ERROR) << "file: " << path
                   << " IR section is invalid: " << e.GetDisplay();
        return false;
      }
    }

    // Lastly perform semantic comparison
    if (!GraphSemanticEqual(qjs_graph, ir_graph)) {
      LOG(ERROR) << "file: result invalid, graph is not semantically equal!";
      LOG(ERROR) << "-------------------------------------------------------";
      LOG(ERROR) << "QJS:";
      LOG(ERROR) << "-------------------------------------------------------";
      LOG(ERROR) << "\n" << qjs_graph.Print();
      LOG(ERROR) << "-------------------------------------------------------";
      LOG(ERROR) << "IR:";
      LOG(ERROR) << "-------------------------------------------------------";
      LOG(ERROR) << "\n" << ir_graph.Print();
      return false;
    }

    return true;
  }
}

}  // namespace _

TEST_CASE("case test", "[ir-case]") {
  auto case_dir = x::TestPathAt("ir/case/");
  LOG(TRACE) << "***************************************************";
  LOG(TRACE) << "Case Dir: " << case_dir;
  LOG(TRACE) << "***************************************************";
  auto total = 0;
  auto fail = 0;

  for (auto& dir_entry :
       std::filesystem::recursive_directory_iterator(case_dir)) {
    if (!dir_entry.is_regular_file()) continue;
    ++total;
    const auto& p = dir_entry.path();
    LOG(TRACE) << "!!! case: " << p.c_str();
    if (!_::TestCase(p.c_str())) {
      LOG(ERROR) << "!!! ***fail***: " << p.c_str();
      ++fail;
    }
  }

  LOG(TRACE) << "total(" << total << "); fail(" << fail << ")";
}

}  // namespace ir
}  // namespace quake
