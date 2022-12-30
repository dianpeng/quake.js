function foo_void() {
  return 42;
}

function foo_unary(a) {
  return a;
}

function foo_binary(a, b) {
  return a + b;
}

function foo_ternary(a, b, c) {
  return a + b + c;
}

function array_sum(arr) {
  let i = 0;
  let sum = 0;
  let l = arr.length;

  for (let i = 0; i < l; ++i) {
    sum += arr[i];
  }
  return sum;
}

function gen_array() {
  let i = 0;
  let arr = [];
  for (let i = 0; i < 18; ++i) {
    arr[i] = i;
  }
  return arr;
}

/**
 * notes(dpeng): Since we have a bug in quake attachment inside of quickjs
 *  for now, ie we attach the JIT into the closure instead of the function
 *  itself, which means every closure creation will/may result in a JIT
 *  compilation. This will be fixed in the future and it should not prevent
 *  us from checking our JIT's correctness though
 */

{
  let result = [];
  let arr = gen_array();
  let value = array_sum(arr);

  for (let i = 0; i < 1000; i++) {
    result[0] = foo_void();
    result[1] = foo_unary(42);
    result[2] = foo_binary(21, 21);
    result[3] = foo_ternary(10, 11, 21);
    yes(value == 153);
  }

  yes(result[0] == 42);
  yes(result[1] == 42);
  yes(result[2] == 42);
  yes(result[3] == 42);
  yes(value == 153);
}
