{
  let sum = 0;
  let X = 0;
  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      sum++;
      if (j > 15) {
        X++;
        break;
      }
    }
  }

  yes(sum == 340);
  yes(X == 20);
}
