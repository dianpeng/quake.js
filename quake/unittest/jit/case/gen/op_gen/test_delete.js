// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_delete_f64_normal() {
  dump("testing: test_delete_f64_normal");
  let a0 = 13.0;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_max() {
  dump("testing: test_delete_f64_max");
  let a0 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_min() {
  dump("testing: test_delete_f64_min");
  let a0 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_nan() {
  dump("testing: test_delete_f64_nan");
  let a0 = NaN;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_pinf() {
  dump("testing: test_delete_f64_pinf");
  let a0 = -Infinity;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_zero() {
  dump("testing: test_delete_f64_zero");
  let a0 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_f64_ninf() {
  dump("testing: test_delete_f64_ninf");
  let a0 = Infinity;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_i32_max() {
  dump("testing: test_delete_i32_max");
  let a0 = 2147483647;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_i32_zero() {
  dump("testing: test_delete_i32_zero");
  let a0 = 0;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_i32_normal() {
  dump("testing: test_delete_i32_normal");
  let a0 = 14;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_i32_min() {
  dump("testing: test_delete_i32_min");
  let a0 = -2147483648;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_bool_max() {
  dump("testing: test_delete_bool_max");
  let a0 = true;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_bool_zero() {
  dump("testing: test_delete_bool_zero");
  let a0 = false;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_bool_normal() {
  dump("testing: test_delete_bool_normal");
  let a0 = true;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_bool_min() {
  dump("testing: test_delete_bool_min");
  let a0 = false;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_undefined_or_null_max() {
  dump("testing: test_delete_undefined_or_null_max");
  let a0 = null;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_undefined_or_null_zero() {
  dump("testing: test_delete_undefined_or_null_zero");
  let a0 = undefined;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_undefined_or_null_normal() {
  dump("testing: test_delete_undefined_or_null_normal");
  let a0 = null;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_undefined_or_null_min() {
  dump("testing: test_delete_undefined_or_null_min");
  let a0 = undefined;
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_object_max() {
  dump("testing: test_delete_object_max");
  let a0 = {'a':1};
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_object_zero() {
  dump("testing: test_delete_object_zero");
  let a0 = {};
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_object_normal() {
  dump("testing: test_delete_object_normal");
  let a0 = {'a':100};
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_object_min() {
  dump("testing: test_delete_object_min");
  let a0 = {};
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_string_max() {
  dump("testing: test_delete_string_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_string_zero() {
  dump("testing: test_delete_string_zero");
  let a0 = '';
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_string_normal() {
  dump("testing: test_delete_string_normal");
  let a0 = 'aaaabbccc';
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_delete_string_min() {
  dump("testing: test_delete_string_min");
  let a0 = '';
  let test_value = undefined;
  let expect = delete a0;
  for (let i = 0; i < 50; i++) {
    test_value = delete a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}
  test_delete_f64_normal();
  test_delete_f64_max();
  test_delete_f64_min();
  test_delete_f64_nan();
  test_delete_f64_pinf();
  test_delete_f64_zero();
  test_delete_f64_ninf();
  test_delete_i32_max();
  test_delete_i32_zero();
  test_delete_i32_normal();
  test_delete_i32_min();
  test_delete_bool_max();
  test_delete_bool_zero();
  test_delete_bool_normal();
  test_delete_bool_min();
  test_delete_undefined_or_null_max();
  test_delete_undefined_or_null_zero();
  test_delete_undefined_or_null_normal();
  test_delete_undefined_or_null_min();
  test_delete_object_max();
  test_delete_object_zero();
  test_delete_object_normal();
  test_delete_object_min();
  test_delete_string_max();
  test_delete_string_zero();
  test_delete_string_normal();
  test_delete_string_min();

}
// ----------------------------------------------------------------