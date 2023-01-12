#include "cg/x64/code-gen.h"
#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/opcode.h"

#include "cg/x64/lir-translate.h"
#include "cg/x64/runtime-gen.h"
#include "cg/x64/stub-gen.h"
#include "cg/x64/abi.h"

#include "quake-rt.h"

#define __ (this->as())->

namespace quake {
namespace cg {
namespace x64 {
namespace {

struct CodeGenErrorHandler : public asmjit::ErrorHandler {
 public:
  void handleError(asmjit::Error err, const char* msg,
                   asmjit::BaseEmitter*) override {
    LOG(FATAL) << "ASMJIT(codegen): " << msg << " (" << err << ")";
  }
};

}  // namespace

CodeGen::CodeGen(Quake* q, lir::HAssembler* as)
    : quake_(q),
      has_(as),
      state_(CodeGen::kNotGen),
      /** dependent state **/
      code_(std::make_unique<CodeX64>(this)),
      as_(std::make_unique<MacroAssembler>(this)) {
  code_->SetUp(this);
}

bool CodeGen::ShouldIgnoreInstruction(lir::HInstruction* ins) const {
  switch (ins->opcode()) {
    case lir::HOpcode::kSentry:
    case lir::HOpcode::kPlaceholder:
      UNREACHABLE() << ins->Print();
      break;

    case lir::HOpcode::kBarrier:
    case lir::HOpcode::kBlock:
    case lir::HOpcode::kStart:
    case lir::HOpcode::kEnd:
    case lir::HOpcode::kBB1:
    case lir::HOpcode::kSlowBlockStart:
    case lir::HOpcode::kSlowBlockEnd:
      break;

    case lir::HOpcode::kPhiLoopF:
    case lir::HOpcode::kPhiLoopI:
    case lir::HOpcode::kPhiLoopPtr:

    case lir::HOpcode::kPhiBranchF:
    case lir::HOpcode::kPhiBranchI:
    case lir::HOpcode::kPhiBranchPtr:
    case lir::HOpcode::kPhiBranchBox:
      break;

    case lir::HOpcode::kSnapshot:
      break;

    case lir::HOpcode::kBranchTrue:
    case lir::HOpcode::kBranchFalse:
      UNREACHABLE() << "BranchTrue/BranchFalse should be eliminated already";
      break;

    default:
      return false;  // cannot be ingored, but have to be generated
  }

  return true;
}

base::Error CodeGen::GenerateBody() {
  // For each instruction inside of the HAssembler, we visiting them one by one
  // and generate instruction for them, we can ignore certain instruction which
  // cannot be generated along the way

  base::Error gen_error;

  // Used to resolve label binding implicitly
  std::unordered_multimap<lir::HInstruction*, lir::Label*> label_map;

  // Collecting all the operand's label
  has_->ForeachInstruction([&](lir::HInstruction* ins) {
    // check label, notes the output cannot be label for now
    for (int i = 0; i < ins->opr_size(); i++) {
      auto opr = ins->Opr(i);
      if (opr->IsLabel()) {
        auto label = opr->GetLabel();
        CHECK(label->HasBind());
        CHECK(!label->HasMachineLabel());
        label->set_machine_label(as()->newLabel());
        auto bind = label->bind();
        label_map.insert(std::make_pair(bind, label));
      }
    }
  });

  auto bind_label = [&](lir::HInstruction* ins) {
    auto [iter_start, iter_end] = label_map.equal_range(ins);

    // check whether we have instruction that can be resolved to bind or not
    for (auto itr = iter_start, end = iter_end; itr != end; itr++) {
      CHECK(itr->second->bind() == itr->first);
      as()->bind(itr->second->machine_label());
    }
    label_map.erase(iter_start, iter_end);
  };

  if (!has_->ForeachInstructionStoppable([&](lir::HInstruction* ins) -> bool {
        // handle instruction's label
        bind_label(ins);

        // check whether we should ignore the instruction or not
        if (ShouldIgnoreInstruction(ins)) return true;

        // translate the instruction
        {
          COMMENT(this, ins->Print().c_str());
          if (auto e = TranslateLIR(quake_, this, has_, ins); e) {
            gen_error = e;
            return false;
          }
        }
        return true;
      })) {
    CHECK(gen_error);
    return gen_error;
  }

  // CHECK(label_map.empty());

  return {};
}

base::Error CodeGen::GenerateOtherStub() {
  {
    __ bind(code_->deopt_->label());

    COMMENT(this, "**** Deoptimization Stub ****");
    Stub::Deopt(this);
    __ unreachable(QuakeJit::kUnreachableEndOfDeoptimization);
  }

  {
    __ bind(code_->cast_f64_to_b32_->label());

    COMMENT(this, "**** CastF64ToB32 ****");
    Stub::CastF64ToB32(this);
    __ unreachable(QuakeJit::kUnreachableEndOfCastF64ToB32);
  }

  {
    __ bind(code_->object_field_get_atom_fast().label());
    COMMENT(this, "**** ObjectFieldGetAtomFast ****");
    Stub::ObjectFieldGetAtomFast(this);

    // FIXME(dpeng)
    __ unreachable(QuakeJit::kUnreachableEndOfCastF64ToB32);
  }

  {
    __ bind(code_->global_get_global_atom_fast().label());
    COMMENT(this, "**** GlobalGetGlobalAtomFast ****");
    Stub::GlobalGetGlobalAtomFast(this);

    // FIXME(dpeng)
    __ unreachable(QuakeJit::kUnreachableEndOfCastF64ToB32);
  }

  // genreate deoptimization
  { GenerateDeopt(); }

  return {};
}

base::Error CodeGen::Generate() {
  CodeGenErrorHandler eh;
  code_->code_holder().setErrorHandler(&eh);

  // generate jump label for all the deoptimization code initially
  for (const auto& deopt : quake()->deopt_list()) {
    code_->AddDeoptEntry(deopt.deopt_id(), __ newLabel());
  }

  // function prologue
  {
    __ bind(code_->jit_prologue_->label());

    COMMENT(this, "**** Prologue of Functions ****");
    Stub::Prologue(this);

    // We jump to the body part, instead of fall-through to allow runtime
    // assertion/inspection
    __ jmp(code_->body_->label());

    __ unreachable(QuakeJit::kUnreachableEndOfPrologue);
  }

  // genereate the body at first
  {
    __ bind(code_->body_->label());
    COMMENT(this, "**** JIT function body ****");
    if (auto e = GenerateBody(); e) {
      state_ = kGenErr;
      return e;
    }
    __ unreachable(QuakeJit::kUnreachableEndOfBody);
  }

  // function epilogue
  {
    __ bind(code_->jit_epilogue_->label());

    COMMENT(this, "**** Epilogue of Functions ****");
    Stub::Epilogue(this);
    __ unreachable(QuakeJit::kUnreachableEndOfEpilogue);
  }

  // (3) Other stub related
  if (auto e = GenerateOtherStub(); e) {
    state_ = kGenErr;
    return e;
  }

  __ bind(code_->end_->label());

  // Lastly static number (embedding)
  code_->EmbedConstPool(this);

  // Finalize the code generation, ie linking etc ...
  if (auto e = Finalize(); e) {
    state_ = kGenErr;
    return e;
  }

  state_ = kGenDone;
  return {};
}

base::Error CodeGen::GenerateDeopt() {
  for (const auto& deopt : quake()->deopt_list()) {

    // We just need branch deoptimization, since they are used for linking each
    // trace during trace transition to side trace etc ... Others are not cared
    if (!deopt.CanBeTraceLink()) continue;

    auto maybe_label = code_->ResolveDeoptEntry(deopt.deopt_id());
    CHECK(maybe_label);

    __ bind(maybe_label->entry.label());
    COMMENT(this, "**** Deoptimization(%d) ****", deopt.deopt_id());
    Stub::DeoptimizeFromTrace(this, deopt);
    __ unreachable(QuakeJit::kUnreachableEndOfDeoptimization);
  }
  return {};
}

std::size_t CodeGen::stack_reserve_size() const {
  static constexpr std::size_t kTempStack = 8 * kStackTempSize;

  auto storage_size = base::Align(
      static_cast<std::size_t>(has_->stack_slot()->stack_byte_size()),
      kStackAlignment);

  return storage_size + kPadding + kTempStack;
}

std::size_t CodeGen::stack_temp_offset() const {
  return base::Align(
             static_cast<std::size_t>(has_->stack_slot()->stack_byte_size()),
             kStackAlignment) +
         kPadding;
}

base::Error CodeGen::Finalize() {
  as()->finalize();
  return code_->Link(this);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
