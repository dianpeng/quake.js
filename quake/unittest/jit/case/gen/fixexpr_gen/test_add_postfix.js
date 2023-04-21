// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_add_postfix_f64_normal() {
  dump("testing: test_add_postfix_f64_normal");

  let original = 13.0;
  let a1 = 13.0;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 13.0;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_max() {
  dump("testing: test_add_postfix_f64_max");

  let original = 1.79769313486e+308;
  let a1 = 1.79769313486e+308;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 1.79769313486e+308;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_min() {
  dump("testing: test_add_postfix_f64_min");

  let original = -1.79769313486e+308;
  let a1 = -1.79769313486e+308;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = -1.79769313486e+308;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_nan() {
  dump("testing: test_add_postfix_f64_nan");

  let original = NaN;
  let a1 = NaN;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = NaN;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_pinf() {
  dump("testing: test_add_postfix_f64_pinf");

  let original = -Infinity;
  let a1 = -Infinity;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = -Infinity;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_zero() {
  dump("testing: test_add_postfix_f64_zero");

  let original = 2.22507385851e-308;
  let a1 = 2.22507385851e-308;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 2.22507385851e-308;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_f64_ninf() {
  dump("testing: test_add_postfix_f64_ninf");

  let original = Infinity;
  let a1 = Infinity;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = Infinity;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_i32_max() {
  dump("testing: test_add_postfix_i32_max");

  let original = 2147483647;
  let a1 = 2147483647;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 2147483647;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_i32_zero() {
  dump("testing: test_add_postfix_i32_zero");

  let original = 0;
  let a1 = 0;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 0;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_i32_normal() {
  dump("testing: test_add_postfix_i32_normal");

  let original = 14;
  let a1 = 14;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 14;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_i32_min() {
  dump("testing: test_add_postfix_i32_min");

  let original = -2147483648;
  let a1 = -2147483648;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = -2147483648;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_string_max() {
  dump("testing: test_add_postfix_string_max");

  let original = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_string_zero() {
  dump("testing: test_add_postfix_string_zero");

  let original = '';
  let a1 = '';

  let test_after = undefined;
  let test_value = undefined;

  let a0 = '';
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_string_normal() {
  dump("testing: test_add_postfix_string_normal");

  let original = 'aaaabbccc';
  let a1 = 'aaaabbccc';

  let test_after = undefined;
  let test_value = undefined;

  let a0 = 'aaaabbccc';
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_string_min() {
  dump("testing: test_add_postfix_string_min");

  let original = '';
  let a1 = '';

  let test_after = undefined;
  let test_value = undefined;

  let a0 = '';
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_bool_max() {
  dump("testing: test_add_postfix_bool_max");

  let original = true;
  let a1 = true;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = true;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_bool_zero() {
  dump("testing: test_add_postfix_bool_zero");

  let original = false;
  let a1 = false;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = false;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_bool_normal() {
  dump("testing: test_add_postfix_bool_normal");

  let original = true;
  let a1 = true;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = true;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_bool_min() {
  dump("testing: test_add_postfix_bool_min");

  let original = false;
  let a1 = false;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = false;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_undefined_or_null_max() {
  dump("testing: test_add_postfix_undefined_or_null_max");

  let original = null;
  let a1 = null;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = null;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_undefined_or_null_zero() {
  dump("testing: test_add_postfix_undefined_or_null_zero");

  let original = undefined;
  let a1 = undefined;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = undefined;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_undefined_or_null_normal() {
  dump("testing: test_add_postfix_undefined_or_null_normal");

  let original = null;
  let a1 = null;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = null;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}


function test_add_postfix_undefined_or_null_min() {
  dump("testing: test_add_postfix_undefined_or_null_min");

  let original = undefined;
  let a1 = undefined;

  let test_after = undefined;
  let test_value = undefined;

  let a0 = undefined;
  let expect_after = a0++;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {
    test_after = a1++;
    test_value = a1;

    a1 = original;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect_value));
  } else {
    yes(test_value === expect_value);
  }

  if (isNaN(test_after)) {
    yes(isNaN(expect_after));
  } else {
    yes(test_after === expect_after);
  }
}
  test_add_postfix_f64_normal();
  test_add_postfix_f64_max();
  test_add_postfix_f64_min();
  test_add_postfix_f64_nan();
  test_add_postfix_f64_pinf();
  test_add_postfix_f64_zero();
  test_add_postfix_f64_ninf();
  test_add_postfix_i32_max();
  test_add_postfix_i32_zero();
  test_add_postfix_i32_normal();
  test_add_postfix_i32_min();
  test_add_postfix_string_max();
  test_add_postfix_string_zero();
  test_add_postfix_string_normal();
  test_add_postfix_string_min();
  test_add_postfix_bool_max();
  test_add_postfix_bool_zero();
  test_add_postfix_bool_normal();
  test_add_postfix_bool_min();
  test_add_postfix_undefined_or_null_max();
  test_add_postfix_undefined_or_null_zero();
  test_add_postfix_undefined_or_null_normal();
  test_add_postfix_undefined_or_null_min();

}
// ----------------------------------------------------------------