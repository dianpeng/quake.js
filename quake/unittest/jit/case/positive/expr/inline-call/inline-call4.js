{
  function foo(a, b) {
    return a + (b === undefined ? 0 : 1);
  }

  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(10) + 1;
  }
  dump(sum);
  yes(sum === 11);
}
