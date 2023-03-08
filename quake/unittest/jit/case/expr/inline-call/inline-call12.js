let G = 100;
{
  function foo(a, b, c) {
    return bar(a, b+c);
  }

  function bar(a, b) {
    return foobar(a+b);
  }

  function foobar(idx) {
    const arr = [1, 2, 3, 4, 5, 
                 1, 2, 3, 4, 5,
                 1, 2, 3, 4, 5,
                 1, 2, 3, 4, 5,
                 1, 2, 3, 4, 5];

    let yyy = G + idx;
    G = yyy;
    return arr[yyy];
  }

  let sum = 0;
  for (let i = 0; i < 1000; i++) {
    sum = foo(1, -2, 2);
  }
  yes(sum === undefined);
  no(sum === null);
}
