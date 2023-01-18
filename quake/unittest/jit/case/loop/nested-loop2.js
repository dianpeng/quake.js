{
  let sumX = 0;
  let sumY = 0;
  for (let i = 0; i < 20; i++) {
    sumY++;
    for (let j = 0; j < 20; j++) {
      sumX++;
    }
    sumY++;
  }

  yes(sumX == 400);
  yes(sumY == 40);
}
