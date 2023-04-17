function foo() {
  return 3;
}

{
  let xxx = 100;
  for (let i = 0; i < 100; i++) {
    sum = foo();
    sum *= xxx;
  }
  yes(sum === 300);
}

