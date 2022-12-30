{
  let sum = 0;
  for (let i = 0; i < 1000; i++) {
    if (i == 100) break;
    sum = i;
  }

  yes(sum == 99);
}
