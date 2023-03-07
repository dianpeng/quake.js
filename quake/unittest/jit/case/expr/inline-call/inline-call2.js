{
  function foo(a) {
    return a;
  };

  let bar = 100;
  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(bar) * 2;
  }
  yes(sum === 200);
}
