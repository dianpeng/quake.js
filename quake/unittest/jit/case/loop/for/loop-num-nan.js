{
  function foo(a, b) {
    return a.a - b.a;
  }
  let sum = 0;
  let o = {};

  for (let i = 0; i < 200; i++) {
    sum += foo(o, o);
  }
  yes(isNaN(sum));
}
