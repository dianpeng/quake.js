function foo(a1, a2, a3) {
  for (let i = 0; i < 1000; i++) {
    a1 = 10;
    a2 = 20;
    a3 = i;
  }
  return a1 + a2 + a3;
}

yes(foo(1, 2, 3) == 10 + 20 + 999);
