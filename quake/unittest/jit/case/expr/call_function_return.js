function return_num() {
  return 42;
}

function return_obj() {
  let x = new Object();
  x.a = 42;
  return x;
}

function return_arr() {
  let x = [];
  x[0] = 42;
  return x;
}

function return_foo() {
  return function() {
    return 42;
  };
}

function return_bool() {
  return true;
}

{
  let result = [];
  for (let i = 0; i < 1000; ++i) {
    result[0] = return_num();
    result[1] = return_obj().a;
    result[2] = return_arr()[0];
    result[3] = return_foo()();
    result[4] = return_bool();
  }

  yes(result[0] == 42);
  yes(result[1] == 42);
  yes(result[2] == 42);
  yes(result[3] == 42);
  yes(result[4]);
}
