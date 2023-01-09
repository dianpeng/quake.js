{
  let result = 0;
  let foo = function() {
    return 42;
  };

  // FIXME(dpeng):
  //   notes, this will not work since fclosure bytecode is not implemented up
  //   to now.
  for (let i = 0; i < 15; i++) {
    let tmp0 = ((a, b) => {return a + b})(1, 2);
    let tmp1 = ((a) => {return a.a + a.b})({'a': 1, 'b': 2});
    result = foo() + tmp0 - tmp1;
  }

  yes(result == 42);
}
