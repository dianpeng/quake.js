{
  let i = 0;
  function foo() {}
  function bar() {}
  function xxx() {}
  while (true) {
    if (i == 100) {
      foo();
      bar();
      xxx();
      break;
    }
    i++;
  }
  yes(i == 100);
}
