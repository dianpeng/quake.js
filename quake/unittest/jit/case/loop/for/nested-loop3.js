{
  let sum = 0;
  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      sum++;
    }
    sum++;
  }
  yes(sum === 420);
}
