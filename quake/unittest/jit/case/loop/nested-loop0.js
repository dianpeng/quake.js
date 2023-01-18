{
  let start = now();
  let sum = 0;
  for (let i = 0; i < 10000; i++) {
    for (let j = 0; j < 10000; j++) {
      sum++;
    }
  }
  let end = now();
  dump("time(10000 * 10000 nested loop): ", end-start);
  yes(sum == 100000000);
}
