function foo(a, b, c, d, e) {
  return bar(a) + foobar(bar(b)) + bar(b) + bar(c) + bar(d) + foobar(bar(e));
}

function bar(a) {
  return a.a;
}

function foobar(a) {
  return a + 10;
}

{
  let obj = {'a': 10};

  let sum = 0;
  for (let i = 0; i < 100; i++) {
    sum = foo(obj, obj, obj, obj, obj);
  }

  yes(sum === 80);
}
