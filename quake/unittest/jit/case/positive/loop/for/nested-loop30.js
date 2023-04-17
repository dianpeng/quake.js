{
  function foo() {
    return 10;
  }
  function bar() {
    return 9;
  }

  let sum = 0;

  for (let i = 0; i < 100; i++) {
    for (let j = 0; j < 100; j++) {
      sum += foo() - bar();
    }
  }

  yes(sum == 100*100);
}
