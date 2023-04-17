{
  let sum1 = 0;
  let sum2 = 0;
  let sum3 = 0;
  let sum4 = 0;

  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      for (let k = 0; k < 20; k++) {
        for (let z = 0; z < 20; z++) {
          sum4++;
        }
        sum3++;
      }
      sum2++;
    }
    sum1++;
  }

  yes(sum1 === 20);
  yes(sum2 === 20 * 20);
  yes(sum3 === 20 * 20 * 20);
  yes(sum4 === 20 * 20 * 20 * 20);
}
