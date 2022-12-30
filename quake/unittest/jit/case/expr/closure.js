{
  let a = 10;
  let foo = function() {
    return a;
  }

  for (let i = 0; i < 1000; ++i) {
    yes(foo() == 10);
  }
}
