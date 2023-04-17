{
  let sum1 = 0;
  let sum2 = 0;
  let sum3 = 0;
  let sum4 = 0;

  for (let i = 0; i < 20; i++) {
    sum1++;
    for (let j = 0; j < 20; j++) {
      sum2++;
      for (let k = 0; k < 20; k++) {
        sum3++;
        for (let z = 0; z < 20; z++) {
          sum4++;
          sum4++;
        }
        sum3++;
      }
      sum2++;
    }
    sum1++;
  }

  yes(sum1 === 20 * 2);
  yes(sum2 === 20 * 20 * 2);
  yes(sum3 === 20 * 20 * 20 * 2);
  yes(sum4 === 20 * 20 * 20 * 20 * 2);
}
