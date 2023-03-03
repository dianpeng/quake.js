{
  function foo(len) {
    let sum = 0;
    for (let i = 0; i < len; i++) {
      for (let j = 0; j < len; j++) {
        for (let z = 0; z < len; z++) {
          sum++;
        }
      }
    }
    return sum;
  }

  let sum = 0;
  let start = now();
  for (let i = 0; i < 10000; i++) {
    sum += foo(20);
  }
  let end = now();
  dump('time: ' + (end - start));
  yes(20 * 20 * 20 * 10000 == sum);
}
