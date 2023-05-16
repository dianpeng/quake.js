{
  function foo(a, b, c, d) {
    a = [1, 2, 3, 4];
    b = {};
    c = {'a': d};
    d = [1, 2];
  }
  for (let i = 0; i < 20; i++) {
    foo(1, 2, 3, 4);
  }
}

{
  function foo(a, b) {
    a = [1, 2, 3, 4];
    b = {};
  }
  for (let i = 0; i < 12; i++) {
    foo(1);
  }
}
