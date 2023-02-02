let sum = 0;

for (let i = 0; i < 20; i++) {
  for (let j = 0; j < 20; j++) {
    for (let x0 = 0; x0 < 20; x0++) {
      sum++;
    }
    for (let x1 = 0; x1 < 20; x1++) {
      sum++;
    }
  }
  for (let z = 0; z < 20; z++) {
    sum++;
  }
  sum++;
}

yes(sum === 16420);
