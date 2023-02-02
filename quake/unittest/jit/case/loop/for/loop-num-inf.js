// number overflow, should result in Infinity

{
  let sum = 1;
  for (let i = 0; i < 1000; i++) {
    sum *= 10001111100111000;
  }
  yes(sum === Infinity);
}
