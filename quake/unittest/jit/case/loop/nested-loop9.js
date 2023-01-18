{
  let sum = 0;
  let start = now();
  for (let i = 0; i < 1000; i++) {
    for (let j = 0; j < 1000; j++) {
      for (let k = 0; k < 1000; k++) {
        sum++;
      }
    }
  }
  let end = now();
  yes(sum == 1000 * 1000 * 1000);
  dump("TIME: ", (end-start));
}
