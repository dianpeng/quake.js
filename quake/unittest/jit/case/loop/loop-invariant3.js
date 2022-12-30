
function bar(a, b, c, d) {
  return a.a + b.a + c.a + d.a;
}

function foo(a, b, c, d) {
  let result = false;

  for (let i = 0; i < 10000; i++) {
    result = bar(a, b, c, d);
  }

  yes(result === 42);
}

let obj1 = {'a': 10};
let obj2 = {'a': 1};
let obj3 = {'a': 1};
let obj4 = {'a': 30};

foo(obj1, obj2, obj3, obj4);
