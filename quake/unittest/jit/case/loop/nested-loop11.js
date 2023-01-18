{
  let i = 0;
  let j = 0;
  let k = 0;
  let sum = 0;
  while (true) {
    if (i >= 20) {
      i = 0;
      break;
    }
    ++i;

    j = 0;
    while (true) {
      if (j >= 20) {
        break;
      }
      ++sum;
      ++j;
    }
  }
  yes(i == 0);
  yes(j == 20);
  yes(sum == 400);
}
