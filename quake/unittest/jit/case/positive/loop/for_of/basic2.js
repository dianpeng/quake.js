function genArray(length) {
  return new Array(length).fill(1);
}

{
  let x = genArray(1000*1000);
  let sum = 0;

  let start = now();
  for (const value of x) {
    sum += value;
  }
  let end = now();

  yes(sum == 1000*1000);
  dump("for of (1000*1000): ", end - start);
}

{
  let start = now();
  let sum = 0;
  let value = 1;
  for (let i = 0; i < 1000*1000; ++i) {
    sum += value;
  }
  let end = now();

  yes(sum == 1000*1000);
  dump("for let (1000*1000): ", end - start);
}
