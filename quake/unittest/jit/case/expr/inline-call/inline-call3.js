{
  function foo(a, b) {
    return a + b;
  };

  let bar = 51;
  let foobar = 49;

  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(bar, foobar) * 2;
  }
  yes(sum === 200);
}
