let foofoo = 10; // global variable

{
  function foo(a, b) {
    let sum1 = foofoo;
    return sum1 + a + b;
  }

  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(1, 1);
  }
  yes(sum === 12);
}
