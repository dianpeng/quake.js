function genArray(length) {
  return new Array(length).fill(1);
}

{
  let sum1 = 0;
  let sum2 = 0;
  let sum3 = 0;

  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      for (let k = 0; k < 20; k++) {
        ++sum1;
      }
      ++sum2;
    }
    ++sum3;
  }

  dump(sum1);
  dump(sum2);
  dump(sum3);
}
/*
{
  const arr1 = genArray(20);
  const arr2 = genArray(20);
  const arr3 = genArray(20);

  let sum1 = 0;
  let sum2 = 0;
  let sum3 = 0;

  for (const x of arr1) {
    for (const y of arr2) {
      for (const z of arr3) {
        ++sum1;
      }
      ++sum2;
    }
    ++sum3;
  }

  dump(sum1);
  dump(sum2);
  dump(sum3);
}
*/
