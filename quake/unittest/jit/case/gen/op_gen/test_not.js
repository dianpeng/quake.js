// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_not_f64_normal() {
  dump("testing: test_not_f64_normal");
  let a0 = 13.0;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_max() {
  dump("testing: test_not_f64_max");
  let a0 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_min() {
  dump("testing: test_not_f64_min");
  let a0 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_nan() {
  dump("testing: test_not_f64_nan");
  let a0 = NaN;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_pinf() {
  dump("testing: test_not_f64_pinf");
  let a0 = Infinity;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_zero() {
  dump("testing: test_not_f64_zero");
  let a0 = 0.0;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_f64_ninf() {
  dump("testing: test_not_f64_ninf");
  let a0 = -Infinity;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_i32_max() {
  dump("testing: test_not_i32_max");
  let a0 = 2147483647;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_i32_zero() {
  dump("testing: test_not_i32_zero");
  let a0 = 0;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_i32_normal() {
  dump("testing: test_not_i32_normal");
  let a0 = 14;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_i32_min() {
  dump("testing: test_not_i32_min");
  let a0 = -2147483648;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_bool_max() {
  dump("testing: test_not_bool_max");
  let a0 = true;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_bool_zero() {
  dump("testing: test_not_bool_zero");
  let a0 = false;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_bool_normal() {
  dump("testing: test_not_bool_normal");
  let a0 = true;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_bool_min() {
  dump("testing: test_not_bool_min");
  let a0 = false;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_undefined_or_null_max() {
  dump("testing: test_not_undefined_or_null_max");
  let a0 = null;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_undefined_or_null_zero() {
  dump("testing: test_not_undefined_or_null_zero");
  let a0 = undefined;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_undefined_or_null_normal() {
  dump("testing: test_not_undefined_or_null_normal");
  let a0 = null;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_not_undefined_or_null_min() {
  dump("testing: test_not_undefined_or_null_min");
  let a0 = undefined;
  let test_value = undefined;
  let expect = ! a0;
  for (let i = 0; i < 50; i++) {
    test_value = ! a0;
  }
  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}
  test_not_f64_normal();
  test_not_f64_max();
  test_not_f64_min();
  test_not_f64_nan();
  test_not_f64_pinf();
  test_not_f64_zero();
  test_not_f64_ninf();
  test_not_i32_max();
  test_not_i32_zero();
  test_not_i32_normal();
  test_not_i32_min();
  test_not_bool_max();
  test_not_bool_zero();
  test_not_bool_normal();
  test_not_bool_min();
  test_not_undefined_or_null_max();
  test_not_undefined_or_null_zero();
  test_not_undefined_or_null_normal();
  test_not_undefined_or_null_min();

}
// ----------------------------------------------------------------