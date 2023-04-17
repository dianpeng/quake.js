let GGG = 100;
{
  function foo(a, b, c) {
    return bar(a, b+c);
  }

  function bar(a, b) {
    return foobar(a+b);
  }

  function foobar(idx) {
    let yyy = GGG + idx;
    GGG = yyy;
    return yyy;
  }

  let start = now();
  let sum = 0;
  for (let i = 0; i < 10000000; i++) {
    sum = foo(1, -2, 2);
  }
  let end = now();
  dump("time: " + (end-start));
  yes(sum === 10000100);
}
