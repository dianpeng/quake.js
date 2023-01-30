#include "cg/x64/stub-gen.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"
#include "cg/x64/deopt-gen.h"
#include "cg/x64/abi.h"

#include "qjs.h"
#include "quake.h"
#include "quake-rt.h"

#define __ (cg->as())->

namespace quake {
namespace cg {
namespace x64 {

namespace a = asmjit;

// eax has deoptimzation id
void Stub::DeoptJumpEntry(CodeGen* cg) {
  // save the deoptimization id into the stack temporary slot
  auto did_target = __ StackTemp0(4);
  __ mov(did_target, SCR_GPD);

  // The following code does deoptimization site selection. The trace_level
  // field inside of the context indicate the current level of enterred jit
  // frame. If it is 1, then just select the common deoptimization site, and
  // if not, we will use special value from context.

  auto default_deopt_label = __ newLabel();
  auto trace_level_addr = a::x86::dword_ptr(CTX, Layout::Context::kTraceLevel);

  __ cmp(trace_level_addr, 1);
  __ je(default_deopt_label);

  // we are in nested loop, we should perform deoptimization site detection
  {
    auto deopt_id_addr = a::x86::dword_ptr(CTX, Layout::Context::kLinkDeoptId);
    __ cmp(SCR_GPD, deopt_id_addr);
    __ jne(default_deopt_label);

    // okay, the deoptimization id matches the expected one, we should
    // deoptimize ourself with specialized deoptimization exit site
    {
      auto addr_off = __ StackTemp1();
      auto address = base::TypePuneCast<std::int64_t>(
          cg->code()->deoptimization_entry_vector());

      // FIXME(dpeng): The following code is too slow since deoptimization
      //   exit for loop exit will be used many times during loop nesting
      //   better get it simple and straitfoward

      // really just calculate the offset and address with a single scratch
      // register
      __ shl(SCR_GPD, 3);
      __ movsxd(SCR_GPQ, SCR_GPD);
      __ mov(addr_off, SCR_GPQ);
      __ movabs(SCR_GPQ, address);
      __ add(SCR_GPQ, addr_off);

      // indirect jumpping
      __ jmp(a::x86::ptr(SCR_GPQ));
    }
  }

  // normal deoptimization site, ie jmp to Deopt
  __ bind(default_deopt_label);
  __ jmp(cg->code()->deopt().label());
}

void Stub::Deopt(CodeGen* cg) {
  auto did = __ StackTemp0(4);
  __ mov(a::x86::eax, did);

  // (0) save all the GP registers, notes order matters
  __ push(a::x86::rax);  // rax is used to save exit#, save it
  __ push(a::x86::rbx);
  __ push(a::x86::rcx);
  __ push(a::x86::rdx);
  __ push(a::x86::rsp);  // nobody care, but need it somehow
  __ push(a::x86::rbp);  // start of the address, will be passed in as args
  __ push(a::x86::rsi);
  __ push(a::x86::rdi);

  __ push(a::x86::r8);
  __ push(a::x86::r9);
  __ push(a::x86::r10);
  __ push(a::x86::r11);
  __ push(a::x86::r12);
  __ push(a::x86::r13);
  __ push(a::x86::r14);
  __ push(a::x86::r15);  // r15 is used to save ctx(QuakeRT)

  // (1) save all the XMM registers, notes order matters, we just perform move
  //     operation on the lower 8 bytes (SD). Notes we have 16 * 8 in total,
  //     which will make sure RSP is aligned with 16 boundary for SSE
  //     requirement

  // notes rax is not used anymore, actually we really do not need it though,
  // but it is simpler for me :)
  __ mov(a::x86::rax, a::x86::rsp);

  // get more spaces for xmm registers since we just do not have PUSHXMM ins
  __ sub(a::x86::rsp, 16 * 8);

  // notes RSP is incorrectly saved on the stack, just get it right for now
  __ sub(a::x86::qword_ptr(a::x86::rax, 8 * 11), 16 * 8);

  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 15), a::x86::xmm0);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 14), a::x86::xmm1);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 13), a::x86::xmm2);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 12), a::x86::xmm3);

  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 11), a::x86::xmm4);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 10), a::x86::xmm5);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 9), a::x86::xmm6);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 8), a::x86::xmm7);

  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 7), a::x86::xmm8);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 6), a::x86::xmm9);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 5), a::x86::xmm10);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 4), a::x86::xmm11);

  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 3), a::x86::xmm12);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 2), a::x86::xmm13);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 1), a::x86::xmm14);
  __ movq(a::x86::qword_ptr(a::x86::rsp, 8 * 0), a::x86::xmm15);

  // (2) Calls into the PrepareDeopt function
  //
  // now all the GP/FP register is free to be used for later on, okay, we can
  // perform the last thing, calling out to the freaking DeoptPrepare function
  //
  // Notes, on x86/64 the stack is growing downwards, so the data been stored
  // inside stack will be related to the arch and not all arch will be the
  // same. One way is the jit code does shuffling to make sure the order is
  // respected, and that is kind of headache since using C++ is far-better.
  // That's the reason each arch will have DeoptData object to help the
  // DeoptPrepare to get the data in a arch independent way. We just make sure
  // our data is been coordinated well with DeoptData implementation in x64
  // folder

  // arg0 = rbp ; ie start of the local stack
  __ mov(a::x86::rdi, a::x86::rbp);

  // arg0 = rsp ; ie end of the local stack
  __ mov(a::x86::rsi, a::x86::rsp);

  // call the DeoptPrepare function
  __ call(FADDR(&QuakeJit::DeoptPrepare));

  // 32 registers(QWORD) are recoveried
  __ add(a::x86::rsp, 32 * 8);

  // (3) Jump to the JIT frame epilogue to exit from JIT back to interpreter
  //
  // jump the epilouge. Notes RAX saves the return value from PrepareDeopt,
  // well may not be meaningful anymore
  __ jmp(cg->code()->epilogue().label());
}

void Stub::Prologue(CodeGen* cg) {
  // make sure that the stack size is aligned with 16 to make SSE happy
  auto stack_size = cg->stack_reserve_size();

  // save callee saved registers, this is really nesty here. We have already
  // decide the size of the stack to be reserved, so any arch-related register
  // saving will not be counted into the stack_size. Instead, this has to be
  // taken care by the code generator. We save all the hidden stuff before the
  // rsp been reserved, and make the rbp points to the region of cell + spill's
  // top address.
  __ push(a::x86::rbp);
  __ push(a::x86::rbx);
  __ push(a::x86::r12);
  __ push(a::x86::r13);
  __ push(a::x86::r14);
  __ push(a::x86::r15);

  __ mov(a::x86::rbp, a::x86::rsp);
  __ sub(a::x86::rsp, a::Imm(stack_size));
  __ mov(CTX, a::x86::rdi);  // rdi contains QuakeRT pointer

  // add book-keeping
  {
    auto trace_level_addr =
        a::x86::dword_ptr(CTX, Layout::Context::kTraceLevel);
    __ add(trace_level_addr, 1);
  }

  // jump to the body of the code
  __ jmp(cg->code()->body().label());
}

// trace prologue, used by trace link
void Stub::TracePrologue(CodeGen* cg) {
  auto stack_size = cg->stack_reserve_size_jit_link();

  {
    auto trace_level_addr =
        a::x86::dword_ptr(CTX, Layout::Context::kTraceLevel);
    __ add(trace_level_addr, 1);
  }

  __ push(a::x86::rbp);
  __ mov(a::x86::rbp, a::x86::rsp);
  __ sub(a::x86::rsp, a::Imm(stack_size));

  __ jmp(cg->code()->body().label());
}

// The epilogue is always jumpping because of deoptimization. And it should
// help to finalize the deoptimization information construction for various
// cases
//
// 1) rax == 0, nothing needs to be done, ie the deoptimization performs the
//    full deoptimization already,
//
// 2) rax != 0
//
//   2.1) If QuakeRT::trace_level is not 1, then just do nothing but return
//        directly, and minus trace_level by 1
//
//   2.2) If QuakeRT::trace_level is 1, then just performs deoptimization
//        rematerialization, but not rematerialize current frame's value
//        since it should already been done correctly

void Stub::Epilogue(CodeGen* cg) {
  // add book-keeping
  {
    auto trace_level_addr =
        a::x86::dword_ptr(CTX, Layout::Context::kTraceLevel);
    __ sub(trace_level_addr, 1);
  }

  // notes this instruction MUST NOT impact flag register
  __ mov(a::x86::rsp, a::x86::rbp);

  // the normal epilogue can be issued during a traced jit link, however it
  // should correctly support different prologue, and correctly skip certain
  // register operation when we are entered by a trace prologue

  auto skip_register_pop = __ newLabel();
  __ jnz(skip_register_pop);

  {
    __ pop(a::x86::r15);
    __ pop(a::x86::r14);
    __ pop(a::x86::r13);
    __ pop(a::x86::r12);
    __ pop(a::x86::rbx);
  }

  __ bind(skip_register_pop);
  __ pop(a::x86::rbp);
  __ ret();
}

void Stub::TraceEpilogue(CodeGen* cg) {
  // add book-keeping
  {
    auto trace_level_addr =
        a::x86::dword_ptr(CTX, Layout::Context::kTraceLevel);
    __ sub(trace_level_addr, 1);
  }

  __ mov(a::x86::rsp, a::x86::rbp);
  __ pop(a::x86::rbp);
  __ ret();
}

/* ----------------------------------------------------------------------------
  union {
    double d;
    uint64 u64;
  } u;

  double d;
  int e;
  u.d = #0; // source

  e = (u.u64 >> 52) & 0x7ff;
  if (likely(e <= (1023 + 30))) {
    ret = (int32_t)d;
  } else if (e <= (1023 + 30 + 53)) {
    uint64_t v;
    v = (u.u64 & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    v = v << ((e - 1023) - 52 + 32);
    ret = v >> 32;
    if (u.u64 >> 63) ret = -ret;
  } else {
    ret = 0;
  }
* ----------------------------------------------------------------------------*/
// Specical function to be used by F64ToB32 stub.
//
// input : xmm15
// scratch: rax, rcx, rbp
//
// stack is aligned to 16 bytesa already, SSE is safe to use, notes, RBP is
// invalid for now, so function unwind is not valid anymore. We may need to
// consider use another reservation register during RA.
void Stub::CastF64ToB32(CodeGen* cg) {
  static constexpr std::uint64_t kShiftConst =
      ((static_cast<std::uint64_t>(1) << 52) - 1) |
      (static_cast<std::uint64_t>(1) << 52);

  // get double's 64 bits representation
  __ movq(SCR_GPQ, SCR_FP);

  __ mov(a::x86::rbp, SCR_GPQ);  // save rbp as alias of input double

  // 2) e = (u.u64 >> 52) & 0xf77
  __ shr(SCR_GPQ, 52);
  __ and_(SCR_GPD, 2047);

  // 3) check the case and dispatch

  auto branch1 = __ newLabel();
  auto branch2 = __ newLabel();
  auto branch3 = __ newLabel();

  // 4) compare e <= 1023 + 30
  __ cmp(SCR_GPD, 1023 + 30);
  __ jbe(branch1);

  // 5) compare e <= 1023 + 30 + 53
  __ cmp(SCR_GPD, 1023 + 30 + 53);
  __ jbe(branch2);
  __ jmp(branch3);

  // 6.1) fast path branch
  __ bind(branch1);

  // ret = (uint32_t)(d);
  {
    __ cvttsd2si(SCR_GPD, SCR_FP);
    __ ret();
  }

  // 6.2) branch avoid fmod
  __ bind(branch2);
  {
    __ mov(a::x86::rcx, SCR_GPQ);         // save e into rcx
    __ movabs(a::x86::rax, kShiftConst);  // load constant
    __ sub(a::x86::rcx, 1023 - 52 + 32);  // e - 1023-52+32
    __ and_(a::x86::rax, a::x86::rbp);    // and #0 & const
    __ sal(a::x86::rax, a::x86::cl);      // v << rcx (e-1023-52+32)
    __ shr(a::x86::rax, 32);              // v >> 32, rax now have ret
    __ mov(a::x86::rcx, a::x86::rax);     // duplicate output retg
    __ neg(a::x86::rcx);                  // negate ret
    __ test(a::x86::rbp, a::x86::rbp);    // testify (f >> 63)
    __ cmovs(a::x86::rax, a::x86::rcx);   // if true uses negated value

    __ ret();
  }

  // 6.3) branch 3, return 0
  __ bind(branch3);
  {
    __ xor_(SCR_GPD, SCR_GPD);
    __ ret();
  }
}

/* ----------------------------------------------------------------------------
 *
 * The following code implements the fast path of function
 * JS_GetPropertyInternal which forms speed up foundation for all the VM
 * bytecode handler that utilize that function.
 *
 * The basic idea is to inline the fast path, which happens most of the time,
 * and fallback to the slow path as runtime call. The slow path is already been
 * taken care of.
 *
 * Parameter passing:
 *
 *   1) SCR_GPQ    : pointer of object
 *   2) StackTemp0 : immediate number of atom
 *
 * Free register:
 *
 *   1) Scratch
 *   2) Rdx
 *
 * ---------------------------------------------------------------------------*/

void Stub::ObjectFieldGetAtomFast(CodeGen* cg) {
  auto atom = __ StackTemp0(4);  // store the atom value
  auto out = __ StackTemp1(4);   // store the output address, cell pointer

  auto temp0 = __ StackTemp2();  // store the shape of object
  auto temp1 = __ StackTemp3();  // store the hash_index value
  auto temp2 = __ StackTemp4();  // store the object pointer
  auto flag = __ StackTemp7(4);  // store the tag value

  auto not_found = __ newLabel();
  auto done = __ newLabel();

  // 0) get the shape pointer and store it into the stack temp1
  __ mov(temp2, SCR_GPQ);
  __ mov(SCR_GPQ, a::x86::qword_ptr(SCR_GPQ, Layout::Object::kShape));
  __ mov(temp0, SCR_GPQ);
  // scratch points to shape pointer

  // 1) figure out the hash_index, ie atom & prop_hash_mask inside of the shape
  __ mov(a::x86::edx, a::x86::dword_ptr(SCR_GPQ, Layout::Shape::kPropHashMask));
  __ and_(a::x86::edx, atom);

  // 2) get the index from hash index
  __ neg(a::x86::edx);
  __ dec(a::x86::edx);
  __ movsxd(a::x86::rdx, a::x86::edx);
  __ mov(a::x86::edx, a::x86::dword_ptr(SCR_GPQ, a::x86::rdx, 2));

  __ dec(a::x86::edx);
  __ movsxd(a::x86::rdx, a::x86::edx);
  __ mov(temp1, a::x86::rdx);

  // now edx contains valid index

  // 3) Get the property array from shape object and load the JSShapeProperty
  //    object into the memory
  {
    auto atom_addr =
        a::x86::dword_ptr(SCR_GPQ,      // shape address
                          a::x86::edx,  // shift
                          3,            // scale, each element is 8 bytes
                          Layout::Shape::kProp + Layout::ShapeProperty::kAtom);

    auto misc_addr = a::x86::byte_ptr(
        SCR_GPQ,      // shape address
        a::x86::edx,  // shift
        3,            // scale, each element is 8 bytes
        Layout::Shape::kProp + Layout::ShapeProperty::kData + 3);

    // 4) Check whether the atom is same or not
    __ mov(SCR_GPD, atom_addr);
    __ cmp(atom, SCR_GPD);
    __ jne(not_found);
    __ mov(SCR_GPQ, temp0);  // restore scratch address

    // 5) Get the corresponding bit value and check whether it is zero or not
    __ movzx(SCR_GPD, misc_addr);
    __ shr(SCR_GPB, 2);
    __ and_(SCR_GPD, 48);
    __ test(SCR_GPD, SCR_GPD);
    __ jnz(not_found);
  }

  // 6) valid branch, ie the property's pointed position is valid, and we need
  //    to get it from object's internal property object

  // Notes(dpeng): This has assumption that JSProperty is 16 bytes. But the
  //   x64 platform addressing mode, does not have 16 scale mode.
  __ sal(a::x86::rdx, 4);  // it should not overflow, since quickjs

  __ mov(SCR_GPQ, temp2);
  __ add(a::x86::rdx, a::x86::qword_ptr(SCR_GPQ, Layout::Object::kProp));

  auto populate_cell_addr = [&]() -> a::x86::Mem {
    // figuring out the cell address based on the cell index
    __ mov(SCR_GPD, out);  // scratch contains cell index
    __ sal(SCR_GPD, 4);    // multiply by 16
    __ neg(SCR_GPD);       // negate it
    __ movsxd(SCR_GPQ, SCR_GPD);

    auto offset = -static_cast<std::int32_t>(Layout::kCellSize) -
                  cg->quake()->cell_offset();

    return a::x86::ptr(asmjit::x86::rbp, SCR_GPQ, 0, offset);
  };

  auto cell_addr = populate_cell_addr();

  // now cell_addr is done, make sure that scratch is not mutated when user
  // need to use cell_address, otherwise the address is not_found anymore

  auto value_addr = a::x86::xmmword_ptr(a::x86::rdx);
  __ movdqu(SCR_FP, value_addr);
  __ movups(cell_addr, SCR_FP);  // xmmword move

  // 7) If the value is a heap object, need to bump its reference counting
  {
    auto tag_addr = cell_addr.cloneAdjusted(Layout::Value::kTag);
    tag_addr.setSize(8);
    __ mov(a::x86::rdx, tag_addr);
    __ cmp(a::x86::rdx, 0);
    __ jge(done);

    // bump the rc
    {
      auto val_addr = cell_addr.cloneAdjusted(Layout::Value::kValue);
      val_addr.setSize(8);
      __ mov(SCR_GPQ, val_addr);
      __ add(a::x86::dword_ptr(SCR_GPQ), 1);
    }
  }

  // 8) everything is done, now rax contains valid result in boxed format,
  //    now we just need to jump out of the specialization part
  __ jmp(done);

  // 8.1) Part that generates the kNotAvailable value into the rax
  __ bind(not_found);
  {
    auto cell_addr = populate_cell_addr();
    auto tag_addr = cell_addr.cloneAdjusted(Layout::Value::kTag);
    tag_addr.setSize(8);

    __ movsxd(a::x86::edx, flag);
    __ mov(tag_addr, a::x86::rdx);

    auto val_addr = cell_addr.cloneAdjusted(Layout::Value::kValue);
    val_addr.setSize(8);
    __ mov(val_addr, 0);
  }

  // 9) done and generate the output
  __ bind(done);

  // 10) return to caller
  __ ret();
}

/* ----------------------------------------------------------------------------
 *
 * This is similar to the ObjectFieldGetAtomFast, instead it needs to check
 * global_var_obj is not valided. For now, all it does is to check whether
 * it contains zero element. The reason is the QuickJS rarely uses this
 * field for now.
 *
 * Parameter passing:
 *
 *   1) SCR_GPQ    : pointer of object
 *   2) StackTemp0 : immediate number of atom
 *
 * Free register:
 *
 *   1) Scratch
 *   2) Rdx
 *
 * ---------------------------------------------------------------------------*/
void Stub::GlobalGetGlobalAtomFast(CodeGen* cg) {
  auto out = __ StackTemp1(4);   // store the output address, cell pointer
  auto flag = __ StackTemp7(4);  // store the tag value

  auto temp0 = __ StackTemp4();  // store SCR_GPQ, notes this should not clobber
                                 // with the one used in ObjectFieldGetAtomFast

  __ mov(temp0, SCR_GPQ);  // save it for later usage
  __ mov(SCR_GPQ, a::x86::qword_ptr(SCR_GPQ, Layout::Object::kShape));
  __ mov(SCR_GPD, a::x86::dword_ptr(SCR_GPQ, Layout::Shape::kPropCount));

  // if the GPD is zero, then just calls back to the ObjectFieldGetAtomFast,
  // otherwise bailout to slow path

  auto not_found = __ newLabel();

  __ test(SCR_GPD, SCR_GPD);
  __ jnz(not_found);

  {
    // rewrite the SCR_GPQ to global_obj field instead of global_var_obj
    auto context = a::x86::qword_ptr(CTX, Layout::Context::kContext);
    auto glb_addr = a::x86::qword_ptr(
        SCR_GPQ, Layout::JSContext::kGlobalObject + Layout::Value::kValue);
    __ mov(SCR_GPQ, context);
    __ mov(SCR_GPQ, glb_addr);
  }
  __ jmp(cg->code()->object_field_get_atom_fast().label());  // tail call

  // okay, we cannot use fast path, but have to bailout. To do this we
  // need to craft a not_found tag
  __ bind(not_found);
  {
    // figuring out the cell address based on the cell index
    __ mov(SCR_GPD, out);  // scratch contains cell index
    __ sal(SCR_GPD, 4);    // multiply by 16
    __ neg(SCR_GPD);       // negate it
    __ movsxd(SCR_GPQ, SCR_GPD);

    auto offset = -static_cast<std::int32_t>(Layout::kCellSize) -
                  cg->quake()->cell_offset();

    auto cell_addr = a::x86::ptr(asmjit::x86::rbp, SCR_GPQ, 0, offset);

    auto tag_addr = cell_addr.cloneAdjusted(Layout::Value::kTag);
    tag_addr.setSize(8);

    __ movsxd(a::x86::rdx, flag);
    __ mov(tag_addr, a::x86::rdx);

    auto val_addr = cell_addr.cloneAdjusted(Layout::Value::kValue);
    val_addr.setSize(8);
    __ mov(val_addr, 0);
    __ ret();
  }
}

void Stub::DeoptimizeFromTrace(CodeGen* cg, const quake::Deopt& deopt) {
  // generate the deoptimization inline assmebly code which optimize for trace
  // linking.
  DeoptGen dgen{cg};
  dgen.Generate(deopt);

  // jump to epilogue, this deoptimization site will only be used for inner
  // jit trace
  __ jmp(cg->code()->trace_epilogue().label());
}

/* ----------------------------------------------------------------------------
 *
 * Assembly implementation of JS_ToBool function. It is basically a fast call
 * of JSValue to boolean conversion.
 *
 * Parameter passing :
 *
 * 1) SCR_GPQ: pointer of JSValue
 *
 * Free register:
 *
 * 1) SCR_GPQ
 * 2) Rdx
 *
 * Return:
 *
 * 1) SCR_GPD
 *
 * ---------------------------------------------------------------------------*/

void Stub::AnyToBool(CodeGen* cg) {
  static const std::uint32_t kMask = (static_cast<std::uint32_t>(1) << 31) - 1;

  auto is_heap = __ newLabel();          // label for heap object
  auto must_true = __ newLabel();        // label for must be true
  auto maybe_must_true = __ newLabel();  // label for must ture and float64
  auto done = __ newLabel();             // label for return

  auto value = __ StackTemp0();

  // save it for future usage
  __ mov(a::x86::rdx, a::x86::qword_ptr(SCR_GPQ, Layout::Value::kValue));
  __ mov(value, a::x86::rdx);

  __ mov(SCR_GPD, a::x86::dword_ptr(SCR_GPQ, Layout::Value::kTag));
  __ cmp(SCR_GPD, JVal::kHeap);
  __ jl(is_heap);

  // primitive types
  {
    __ cmp(SCR_GPD, JVal::kUndefined);
    __ jg(maybe_must_true);

    __ mov(a::x86::edx, value);
    __ xor_(SCR_GPD, SCR_GPD);

    __ test(a::x86::edx, a::x86::edx);
    __ mov(a::x86::edx, 1);
    __ cmovnz(SCR_GPD, a::x86::edx);

    __ jmp(done);
  }

  // maybe must true
  __ bind(maybe_must_true);
  {
    __ cmp(SCR_GPD, JVal::kFloat64);
    __ jne(must_true);

    // float64 conversion
    {
      __ movsd(SCR_FP, value);
      __ comisd(SCR_FP, cg->code()->const_f64_pos_zero());
      __ mov(a::x86::edx, 1);
      __ setp(SCR_GPB);
      __ cmovne(SCR_GPD, a::x86::edx);
      __ jmp(done);
    }
  }

  // heap
  __ bind(is_heap);
  {
    auto try_object = __ newLabel();

    __ cmp(SCR_GPD, JVal::kString);
    __ jne(try_object);

    // string type
    {
      __ mov(SCR_GPQ, value);  // SCR_GPQ is a pointer to string
      __ mov(a::x86::edx, a::x86::dword_ptr(SCR_GPQ, Layout::String::kLength));
      __ and_(a::x86::edx, kMask);
      __ xor_(SCR_GPD, SCR_GPD);
      __ test(a::x86::edx, a::x86::edx);
      __ mov(a::x86::edx, 1);
      __ cmovnz(SCR_GPD, a::x86::edx);
      __ jmp(done);
    }

    __ bind(try_object);
    {
      __ cmp(SCR_GPD, JVal::kObject);
      __ jne(must_true);

      // object
      {
        __ mov(SCR_GPQ, value);

        // The following code is mainly for testing the is_HTMLDDA bit field,
        // and to learn whether it is on or not.
        __ movzx(SCR_GPD, a::x86::byte_ptr(SCR_GPD, 5));
        __ shr(SCR_GPB, 7);
        __ jmp(done);
      }
    }
  }

  // must true
  __ bind(must_true);
  __ mov(SCR_GPD, 1);

  // everything is done
  __ bind(done);
  __ ret();
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
