{
  let sum = 0;
  let start = now();
  for (let z = 0; z < 20; z++) {
    for (let u = 0; u < 100; u++) {
      for (let i = 0; i < 100; i++) {
        for (let j = 0; j < 100; j++) {
          for (let k = 0; k < 100; k++) {
            sum++;
          }
        }
      }
    }
  }
  let end = now();
  dump("TIME: ", (end-start));
  yes(sum == 20 * 100 * 100 * 100 * 100);
}
