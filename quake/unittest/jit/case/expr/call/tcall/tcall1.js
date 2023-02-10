function foo() {
  return bar();
}

function bar() {
  return 10;
}

{
  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo();
  }
  yes(sum === 10);
}
