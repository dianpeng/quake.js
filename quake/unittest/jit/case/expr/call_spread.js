/** spread syntax, ...args */

function foo(a, b, c) {
  return a + b + c;
}

{
  let arg1 = [1,2,3];
  let arg2 = [2, 3];
  let arg3 = [3];

  let result = 0;
  for (let i = 0; i < 100; i++) {
    result = foo(...arg1);
    result -= foo(1, ...arg2);
    result = foo(-1, -2, ...arg3);
  }
  yes(result == 0);
}
