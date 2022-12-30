{
  let sum = 0;
  for (let i = 0; i < 10000; i++) {
    sum = i;
    if (i == 100) break;
  }
  yes(sum == 100);
}
