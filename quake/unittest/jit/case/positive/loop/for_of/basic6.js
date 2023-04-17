function genArray(length) {
  return new Array(length).fill(1);
}

{
  const arr1 = genArray(20);
  const arr2 = genArray(20);
  const arr3 = genArray(20);
  const arr4 = genArray(20);
  const arr5 = genArray(20);
  const arr6 = genArray(20);

  let sum1 = 0;
  let sum2 = 0;
  let sum3 = 0;
  let sum4 = 0;
  let sum5 = 0;
  let sum6 = 0;

  for (const x1 of arr1) {
    ++sum1;
    for (const x2 of arr2) {
      ++sum2;
      for (const x3 of arr3) {
        ++sum3;
        for (const x4 of arr4) {
          ++sum4;
          for (const x5 of arr5) {
            ++sum5;
            for (const x6 of arr6) {
              ++sum6;
              ++sum6;
            }
            ++sum5;
          }
          ++sum4;
        }
        ++sum3;
      }
      ++sum2;
    }
    ++sum1;
  }

  yes(sum1 === 20 * 2);
  yes(sum2 === 400 * 2);
  yes(sum3 === 8000 * 2);
  yes(sum4 === 160000 * 2);
  yes(sum5 === 3200000 * 2);
  yes(sum6 === 64000000 * 2);
}
