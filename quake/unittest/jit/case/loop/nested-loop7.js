function foo(a, b) {
  let sum = 0;
  for (let i = a; i < b; ++i) {
    for (let j = a; j < b; ++j) {
      sum++;
    }
  }
  return sum;
}

yes(foo(0, 20) == 400);
