{
  function foo1(a) {
    return foo2(a);
  }
  function foo2(a) {
    return foo3(a);
  }
  function foo3(a) {
    return foo4(a);
  }
  function foo4(a) {
    return foo5(a);
  }
  function foo5(a) {
    return foo6(a);
  }
  function foo6(a) {
    return a;
  }
  function bar() {
    return 10;
  }

  let start = now();
  let sum = 0;
  for (let i = 0; i < 10000000; i++) {
    sum = foo1(bar)();
  }
  let end = now();
  dump("time: " + (end-start));
  yes(sum === 10);
}
