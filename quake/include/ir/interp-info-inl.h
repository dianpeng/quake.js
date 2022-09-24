#ifndef _QUAKE_INTERP_INFO_INL_H_
#define _QUAKE_INTERP_INFO_INL_H_

namespace quake {
namespace ir {

inline InterpInfo::InterpInfo(Quake* q, QInterpInfo* info)
    : quake_(q),
      info_(info),
      interp_pc_(static_cast<std::uint8_t*>(info_->bc.pc)),
      pc_(static_cast<std::uint8_t*>(info_->bc.pc)),
      pc_op_(static_cast<Bytecode>(info_->bc.op)) {}

inline JVal InterpInfo::GetParam(std::uint32_t index) const {
  CHECK(index < info_->frame.arg_size);
  return JVal{qjs_context(), info_->frame.arg[index]};
}

inline JVal InterpInfo::GetLocal(std::uint32_t index) const {
  CHECK(index < info_->frame.loc_size);
  return JVal{qjs_context(), info_->frame.loc[index]};
}

inline JVal InterpInfo::GetTemp(std::uint32_t index) const {
  CHECK(index < info_->frame.temp_size);
  return JVal{qjs_context(), info_->frame.loc[index]};
}

inline JVal InterpInfo::GetSlot(std::uint32_t index) const {
  CHECK(index < info_->frame.slot_size);
  return JVal{qjs_context(), info_->frame.slot[index]};
}

inline std::uint32_t InterpInfo::local_size() const {
  return info_->frame.loc_size;
}

inline std::uint32_t InterpInfo::param_size() const {
  return info_->frame.arg_size;
}

inline std::uint32_t InterpInfo::temp_size() const {
  return info_->frame.temp_size;
}

inline std::uint32_t InterpInfo::slot_size() const {
  return info_->frame.slot_size;
}

inline JVal InterpInfo::GetConst(std::uint32_t idx) const {
  auto v =
      qjs()->api.bytecode_get_constant(qjs_context(), info_->bc.bc_object, idx);

  return JVal{qjs_context(), v};
}

inline JVal InterpInfo::GetAtom(std::uint32_t idx, bool force_string) const {
  auto v =
      qjs()->api.context_get_atom(qjs_context(), idx, force_string ? 1 : 0);
  return JVal{qjs_context(), v};
}

inline JVal InterpInfo::This() const {
  return JVal{qjs_context(), info_->frame.this_object};
}

inline JVal InterpInfo::Function() const {
  return JVal{qjs_context(), info_->frame.func_object};
}

inline JVal InterpInfo::Target() const {
  return JVal{qjs_context(), info_->frame.target_object};
}

inline JVal InterpInfo::Global() const {
  return JVal{qjs_context(), qjs()->api.context_get_global(qjs_context())};
}

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_INTERP_INFO_INL_H_
