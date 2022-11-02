#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "qjs-bytecode-test-util.h"

#include <quake.h>
#include <quake-rt.h>
#include <jit.h>
#include <ir/builder.h>
#include <ir/graph.h>

#include <lir/assembler.h>
#include <lir/translator.h>
#include <lir/rewriter.h>
#include <lir/register-allocator.h>
#include <lir/pre-code-gen.h>

#include <cg/x64/code-gen.h>

namespace quake {
namespace ir {

TEST_CASE("run the HIR builder", "[builder]") {
  Quake quake{nullptr};
  Graph g{&quake};
  Builder builder{&quake, &g};
  lir::HAssembler as{&quake};

  QJSFunctionBytecode qjs_func;

  qjs_func.SetUp();
  qjs_func.Compile(R"""(

  {
    let out1 = 0;
    for (let i = 0; i < 2; i++) {
      out1 += 1;
    }
  }

)""");

  auto has_start = false;

  qjs_func.ForeachBytecodeInNaturalLoop(
      [&](InterpInfo* info) {
        LOG(ERROR) << info->op_name();
        if (!has_start) {
          has_start = true;
          auto r = builder.Setup(info);
          if (!r) {
            LOG(FATAL) << r.message();
          }
        }
        if (auto r = builder.Trace(info); !r) {
          LOG(FATAL) << r.message();
        }
      },
      [&](InterpInfo* info) { builder.Stop(info); });

  LOG(TRACE) << "--------------------------------------------------------\n";
  LOG(TRACE) << "HIR-----------------------------------------------------\n";
  LOG(TRACE) << '\n' << g.Print();

  {
    lir::Translator trans{&quake, &as};
    trans.Run(&g);
  }

  LOG(TRACE) << "--------------------------------------------------------\n";
  LOG(TRACE) << "LIR-----------------------------------------------------\n";
  LOG(TRACE) << '\n' << as.Print();

  { lir::Rewriter::Rewrite(&quake, &as); }
  LOG(TRACE) << "--------------------------------------------------------\n";
  LOG(TRACE) << "LIR (after rewrite) ------------------------------------\n";
  LOG(TRACE) << '\n' << as.Print();

  {
    lir::RegisterAllocator ra{&quake, &as};
    ra.Run();
  }
  LOG(TRACE) << "--------------------------------------------------------\n";
  LOG(TRACE) << "LIR (after RA) -----------------------------------------\n";
  LOG(TRACE) << '\n' << as.Print();

  {
    lir::PreCodeGen pcg{&quake, &as};
    pcg.Run();
  }
  LOG(TRACE) << "--------------------------------------------------------\n";
  LOG(TRACE) << "LIR (after PCG) ----------------------------------------\n";
  LOG(TRACE) << '\n' << as.Print();

  {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "Code Generation ----------------------------------------\n";

    cg::x64::CodeGen cg{&quake, &as};
    if (auto err = cg.Generate(); err) {
      LOG(FATAL) << err.GetDisplay();
    }
    auto code = cg.DetachCode();
    LOG(ERROR) << code->Disassemble();

    QuakeRT rt;
    JSValue v[3];
    v[0] = __JS_NewFloat64(nullptr, 0.0);
    v[1] = __JS_NewFloat64(nullptr, 0.0);
    v[2] = __JS_NewFloat64(nullptr, 0.0);

    rt.local = v;
    rt.local_end = v + 3;

    auto entry = code->entry();
    entry(&rt);
  }
}

}  // namespace ir
}  // namespace quake
