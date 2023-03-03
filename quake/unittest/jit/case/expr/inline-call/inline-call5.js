{
  function foo(a) {
    let aa = 10;
    let bb = 20;
    return a + aa + bb;
  }

  let sum = 0;
  for (let i = 0; i < 1000; i++) {
    sum = foo(10) + 1;
  }
  dump(sum);
  yes(sum === 41);
}
