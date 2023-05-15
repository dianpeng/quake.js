{
  function foo(a) {
    a = [1, 2, 3, 4];
  }
  jit_trace_on();

  for (let i = 0; i < 12; i++) {
    foo(1);
  }
}
