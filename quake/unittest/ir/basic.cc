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
    let o1 = 0;

    let out1 = 1;
    let out2 = 1;
    let out3 = 1;
    let out4 = 1;
    let out5 = 1;
    let out6 = 1;

    for (let i = 0; i < 10000000; i++) {
      o1 += 1;
      o1 /= 2;

      out1 *= 1;
      out2 += 1;
      out3 += 1;
      out4 += 1;
      out5 += 1;

      out2 *= 1;
      out3 *= 1;
      out4 *= 1;
      out5 *= 1;
    }
  }

)""");

  auto has_start = false;

  qjs_func.ForeachBytecodeInNaturalLoop(
      [&](InterpInfo* info) {
        LOG(ERROR) << info->op_name() << "@(" << info->start_pc() << ")";
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
    rt.context = qjs_func.js_context();
    rt.quake = &quake;
    JSValue v[16];
    for (int i = 0; i < 16; i++) {
      if (i == 2) {
        v[i] = __JS_NewFloat64(nullptr, 2.0);
      } else {
        v[i] = __JS_NewFloat64(nullptr, 1.0);
      }
    }

    rt.local = v;
    rt.local_end = v + 30;

    auto entry = code->entry();
    entry(&rt);
  }
}

}  // namespace ir
}  // namespace quake
