{
  let sum = 0;
  for (let i = 0; i < 20; i++) {
    let sumY = 0;
    let objY = {'x': 0};
    for (let j = 0; j < 20; j++) {
      sumY++;
      sum++;
      objY.x++;
    }
    yes(sumY == 20);
    yes(objY.x == 20);
  }
  yes(sum == 400);
}
