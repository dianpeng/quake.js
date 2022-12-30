{
  let result = 0;
  let foo = function() {
    return 42;
  };

  for (let i = 0; i < 100; i++) {
    let tmp0 = ((a, b) => {return a + b})(1, 2);
    let tmp1 = ((a) => {return a.a + a.b})({'a': 1, 'b': 2});
    result = foo() + tmp0 - tmp1;
  }

  yes(result == 42);
}
