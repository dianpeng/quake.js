# Quake.js: experimental quickjs JIT compiler

# Current Status

- Side project for exploration of jit compiler in quick.js
  - For exploration and testing, not ready for production

# Feature

- A tracing JIT compiler for quick.js
  - automatic reference counting handling
  - loop nests based region selection: a trace
  - 2 levels IR
    - HIR: sea-of-nodes
    - LIR: ssa cfg
  - partial evaluation/specialization
    - type specialization
      - specialize on common set of types not a single type
  - deoptimization via sparse checkpoint tracing
    - interpreter frame restoration via external register mapping
  - lower/optimization
    - HIR: dce/fold/gvn/licm via scheduling
    - LIR: 
      - block motion
      - lower by rewrite
      - phi elimination
      - ...
    - CG:
      - linear scan RA
      - for now x64 only

- Fuzzy tests
  - using jsfuzz (tailered) for testing

- Tech stack
  - c++17
