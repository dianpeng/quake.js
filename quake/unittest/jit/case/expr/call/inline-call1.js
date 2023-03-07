function foo() {
  return 3;
}

{
  let xxx = 100;
  let sum = 0;
  for (let i = 0; i < 20; i++) {
    sum = foo();
    sum *= xxx;
  }
}

