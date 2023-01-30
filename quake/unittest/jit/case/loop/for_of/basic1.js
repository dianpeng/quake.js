
{
  let arr =
      [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20];

  let sum = 0;
  for (let x of arr) {
    sum += x;
  }
  yes(sum === 210);
}
