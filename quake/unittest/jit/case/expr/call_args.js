function foo(a, b, c) {
  return a + b + c;
}
{
  let i = 0;
  let result1 = 0;
  let result2 = 0;

  for (i = 0; i < 1000; ++i) {
    result1 = foo(1, 2, 3);
    result2 = foo(1, 2);
  }
  yes(result1 == 1 + 2 + 3);
  yes(isNaN(result2));
}
