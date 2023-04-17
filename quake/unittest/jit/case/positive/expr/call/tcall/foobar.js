{
  function foo(a, b) {
    return a + b;
  }

  let sum = 0;
  for (let i = 0; i < 20; i++) {
    sum = foo(1, 20);
  }
  yes(sum === 21);
}
