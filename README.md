# Quake.js: experimental quickjs JIT compiler

# Current Status

- Side project for exploration of jit compiler in quick.js
  - For exploration and testing, not ready for production

- What was done
  - (1) Counter based trigger
  - (2) One loop iteration compilation, ie a single trace
  - (3) Deoptimization

- What should be done
  - Cross trace linkage (TODO)
    - This allows full jitting for the whole method

- Is it stable ?
  - NO, several known bugs
    - But should not be too hard to fix
  - Also the quickjs upstream version has memory leak too
  - Code generation generates okay code
    - RA should be splitted base, ie LLVM's greedy allocator style
    - More machine independent optimization pass should be added
    - But it is pretty performant already, like order of magnitude faster than original interpreter

# Feature

- A tracing JIT compiler for quickjs
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

# Roadmap

- I write it sololy for fun
  - I am busy with daily job, so not too much time can be afforded
  - Hopefully, I can get it done and also make it into production status
