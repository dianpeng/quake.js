{
  let sum = 0;
  for (let i = 0; i < 1000; i++) {
    if (i >= 599) break;
    if (i >= 499) break;
    if (i >= 388) break;
    if (i >= 277) break;
    if (i == 100) break;

    sum = i;
  }

  yes(sum == 99);
}
