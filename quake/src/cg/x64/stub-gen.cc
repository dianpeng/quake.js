#include "cg/x64/stub-gen.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"
#include "cg/x64/abi.h"

#include "qjs.h"
#include "quake.h"
#include "quake-rt.h"

#define __ (cg->as())->

namespace quake {
namespace cg {
namespace x64 {

namespace a = asmjit;

void Stub::Deopt(CodeGen* cg) {
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

  // scratch RAX, in case DeoptPrepare return something, (ie modify its
  // signature);
  __ xor_(a::x86::rax, a::x86::rax);

  // call the DeoptPrepare function
  __ call(FADDR(&QuakeJit::DeoptPrepare));

  // 32 registers(QWORD) are recoveried
  __ add(a::x86::rsp, 32 * 8);

  // (3) Jump to the JIT frame epilogue to exit from JIT back to interpreter
  //
  // jump the epilouge. Notes RAX saves the return value from PrepareDeopt,
  // well may not be meaningful anymore
  __ jmp(cg->code()->jit_epilogue().label());
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
}

void Stub::Epilogue(CodeGen* cg) {
  __ mov(a::x86::rsp, a::x86::rbp);

  __ pop(a::x86::r15);
  __ pop(a::x86::r14);
  __ pop(a::x86::r13);
  __ pop(a::x86::r12);
  __ pop(a::x86::rbx);
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

  auto invalid = __ newLabel();
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
  __ movsx(a::x86::rdx, a::x86::edx);
  __ mov(a::x86::edx, a::x86::dword_ptr(SCR_GPQ, a::x86::rdx, 2));

  __ dec(a::x86::edx);
  __ movsx(a::x86::rdx, a::x86::edx);
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
    __ jne(invalid);
    __ mov(SCR_GPQ, temp0);  // restore scratch address

    // 5) Get the corresponding bit value and check whether it is zero or not
    __ movzx(SCR_GPD, misc_addr);
    __ shr(SCR_GPB, 2);
    __ and_(SCR_GPD, 48);
    __ test(SCR_GPD, SCR_GPD);
    __ jnz(invalid);
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
    __ movsx(SCR_GPQ, SCR_GPD);

    auto offset = -static_cast<std::int32_t>(Layout::kCellSize) -
                  cg->quake()->cell_offset();

    return a::x86::ptr(asmjit::x86::rbp, SCR_GPQ, 0, offset);
  };

  auto cell_addr = populate_cell_addr();

  // now cell_addr is done, make sure that scratch is not mutated when user
  // need to use cell_address, otherwise the address is invalid anymore

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
  __ bind(invalid);
  {
    auto cell_addr = populate_cell_addr();
    auto tag_addr = cell_addr.cloneAdjusted(Layout::Value::kTag);
    tag_addr.setSize(8);

    __ mov(a::x86::edx, flag);
    __ movzx(a::x86::rdx, a::x86::edx);
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

}  // namespace x64
}  // namespace cg
}  // namespace quake
