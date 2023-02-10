function foo(a) {
  return bar(a);
}
function bar(a) {
  return a.a;
}
{
  let obj = {'a': 100};
  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(obj);
  }
  yes(sum === 100);
}
