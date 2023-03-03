{
  function foo(len) {
    let sum = 0;
    for (let i = 0; i < len; i++) {
      sum++;
    }
    return sum;
  }

  let sum = 0;
  let start = now();
  for (let i = 0; i <10000; i++) {
    sum += foo(20000);
  }
  let end = now();
  dump("time: " + (end-start));

  yes(20000 * 10000 == sum);
}
