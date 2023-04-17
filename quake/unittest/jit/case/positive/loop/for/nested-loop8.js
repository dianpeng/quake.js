function foo(a, b) {
  for (let i = 0; i < 20; ++i) {
    for (let j = 0; j < 20; ++j) {
      a++;
    }
    b++;
  }
  return a + b;
}

yes(foo(1, 2) == (1+20) + (2+ 400));
