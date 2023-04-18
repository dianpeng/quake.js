// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_eq_f64_f64_max_max() {
  dump("testing: test_eq_f64_f64_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_max_zero() {
  dump("testing: test_eq_f64_f64_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_max_normal() {
  dump("testing: test_eq_f64_f64_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_max_min() {
  dump("testing: test_eq_f64_f64_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_max() {
  dump("testing: test_eq_f64_f64_zero_max");
  let a0 = 0.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_zero() {
  dump("testing: test_eq_f64_f64_zero_zero");
  let a0 = 0.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_normal() {
  dump("testing: test_eq_f64_f64_zero_normal");
  let a0 = 0.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_min() {
  dump("testing: test_eq_f64_f64_zero_min");
  let a0 = 0.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_max() {
  dump("testing: test_eq_f64_f64_normal_max");
  let a0 = 13.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_zero() {
  dump("testing: test_eq_f64_f64_normal_zero");
  let a0 = 13.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_normal() {
  dump("testing: test_eq_f64_f64_normal_normal");
  let a0 = 13.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_min() {
  dump("testing: test_eq_f64_f64_normal_min");
  let a0 = 13.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_max() {
  dump("testing: test_eq_f64_f64_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_zero() {
  dump("testing: test_eq_f64_f64_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_normal() {
  dump("testing: test_eq_f64_f64_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_min() {
  dump("testing: test_eq_f64_f64_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_max_max() {
  dump("testing: test_eq_f64_i32_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_max_zero() {
  dump("testing: test_eq_f64_i32_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_max_normal() {
  dump("testing: test_eq_f64_i32_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_max_min() {
  dump("testing: test_eq_f64_i32_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_zero_max() {
  dump("testing: test_eq_f64_i32_zero_max");
  let a0 = 0.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_zero_zero() {
  dump("testing: test_eq_f64_i32_zero_zero");
  let a0 = 0.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_zero_normal() {
  dump("testing: test_eq_f64_i32_zero_normal");
  let a0 = 0.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_zero_min() {
  dump("testing: test_eq_f64_i32_zero_min");
  let a0 = 0.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_normal_max() {
  dump("testing: test_eq_f64_i32_normal_max");
  let a0 = 13.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_normal_zero() {
  dump("testing: test_eq_f64_i32_normal_zero");
  let a0 = 13.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_normal_normal() {
  dump("testing: test_eq_f64_i32_normal_normal");
  let a0 = 13.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_normal_min() {
  dump("testing: test_eq_f64_i32_normal_min");
  let a0 = 13.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_min_max() {
  dump("testing: test_eq_f64_i32_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_min_zero() {
  dump("testing: test_eq_f64_i32_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_min_normal() {
  dump("testing: test_eq_f64_i32_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_i32_min_min() {
  dump("testing: test_eq_f64_i32_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_max_max() {
  dump("testing: test_eq_f64_string_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_max_zero() {
  dump("testing: test_eq_f64_string_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_max_normal() {
  dump("testing: test_eq_f64_string_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_max_min() {
  dump("testing: test_eq_f64_string_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_zero_max() {
  dump("testing: test_eq_f64_string_zero_max");
  let a0 = 0.0;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_zero_zero() {
  dump("testing: test_eq_f64_string_zero_zero");
  let a0 = 0.0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_zero_normal() {
  dump("testing: test_eq_f64_string_zero_normal");
  let a0 = 0.0;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_zero_min() {
  dump("testing: test_eq_f64_string_zero_min");
  let a0 = 0.0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_normal_max() {
  dump("testing: test_eq_f64_string_normal_max");
  let a0 = 13.0;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_normal_zero() {
  dump("testing: test_eq_f64_string_normal_zero");
  let a0 = 13.0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_normal_normal() {
  dump("testing: test_eq_f64_string_normal_normal");
  let a0 = 13.0;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_normal_min() {
  dump("testing: test_eq_f64_string_normal_min");
  let a0 = 13.0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_min_max() {
  dump("testing: test_eq_f64_string_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_min_zero() {
  dump("testing: test_eq_f64_string_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_min_normal() {
  dump("testing: test_eq_f64_string_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_string_min_min() {
  dump("testing: test_eq_f64_string_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_max_max() {
  dump("testing: test_eq_f64_bool_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_max_zero() {
  dump("testing: test_eq_f64_bool_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_max_normal() {
  dump("testing: test_eq_f64_bool_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_max_min() {
  dump("testing: test_eq_f64_bool_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_zero_max() {
  dump("testing: test_eq_f64_bool_zero_max");
  let a0 = 0.0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_zero_zero() {
  dump("testing: test_eq_f64_bool_zero_zero");
  let a0 = 0.0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_zero_normal() {
  dump("testing: test_eq_f64_bool_zero_normal");
  let a0 = 0.0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_zero_min() {
  dump("testing: test_eq_f64_bool_zero_min");
  let a0 = 0.0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_normal_max() {
  dump("testing: test_eq_f64_bool_normal_max");
  let a0 = 13.0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_normal_zero() {
  dump("testing: test_eq_f64_bool_normal_zero");
  let a0 = 13.0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_normal_normal() {
  dump("testing: test_eq_f64_bool_normal_normal");
  let a0 = 13.0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_normal_min() {
  dump("testing: test_eq_f64_bool_normal_min");
  let a0 = 13.0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_min_max() {
  dump("testing: test_eq_f64_bool_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_min_zero() {
  dump("testing: test_eq_f64_bool_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_min_normal() {
  dump("testing: test_eq_f64_bool_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_bool_min_min() {
  dump("testing: test_eq_f64_bool_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_max_max() {
  dump("testing: test_eq_f64_undefined_or_null_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_max_zero() {
  dump("testing: test_eq_f64_undefined_or_null_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_max_normal() {
  dump("testing: test_eq_f64_undefined_or_null_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_max_min() {
  dump("testing: test_eq_f64_undefined_or_null_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_zero_max() {
  dump("testing: test_eq_f64_undefined_or_null_zero_max");
  let a0 = 0.0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_zero_zero() {
  dump("testing: test_eq_f64_undefined_or_null_zero_zero");
  let a0 = 0.0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_zero_normal() {
  dump("testing: test_eq_f64_undefined_or_null_zero_normal");
  let a0 = 0.0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_zero_min() {
  dump("testing: test_eq_f64_undefined_or_null_zero_min");
  let a0 = 0.0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_normal_max() {
  dump("testing: test_eq_f64_undefined_or_null_normal_max");
  let a0 = 13.0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_normal_zero() {
  dump("testing: test_eq_f64_undefined_or_null_normal_zero");
  let a0 = 13.0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_normal_normal() {
  dump("testing: test_eq_f64_undefined_or_null_normal_normal");
  let a0 = 13.0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_normal_min() {
  dump("testing: test_eq_f64_undefined_or_null_normal_min");
  let a0 = 13.0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_min_max() {
  dump("testing: test_eq_f64_undefined_or_null_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_min_zero() {
  dump("testing: test_eq_f64_undefined_or_null_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_min_normal() {
  dump("testing: test_eq_f64_undefined_or_null_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_undefined_or_null_min_min() {
  dump("testing: test_eq_f64_undefined_or_null_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_max() {
  dump("testing: test_eq_i32_f64_max_max");
  let a0 = 2147483647;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_zero() {
  dump("testing: test_eq_i32_f64_max_zero");
  let a0 = 2147483647;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_normal() {
  dump("testing: test_eq_i32_f64_max_normal");
  let a0 = 2147483647;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_min() {
  dump("testing: test_eq_i32_f64_max_min");
  let a0 = 2147483647;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_max() {
  dump("testing: test_eq_i32_f64_zero_max");
  let a0 = 0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_zero() {
  dump("testing: test_eq_i32_f64_zero_zero");
  let a0 = 0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_normal() {
  dump("testing: test_eq_i32_f64_zero_normal");
  let a0 = 0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_min() {
  dump("testing: test_eq_i32_f64_zero_min");
  let a0 = 0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_max() {
  dump("testing: test_eq_i32_f64_normal_max");
  let a0 = 14;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_zero() {
  dump("testing: test_eq_i32_f64_normal_zero");
  let a0 = 14;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_normal() {
  dump("testing: test_eq_i32_f64_normal_normal");
  let a0 = 14;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_min() {
  dump("testing: test_eq_i32_f64_normal_min");
  let a0 = 14;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_max() {
  dump("testing: test_eq_i32_f64_min_max");
  let a0 = -2147483648;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_zero() {
  dump("testing: test_eq_i32_f64_min_zero");
  let a0 = -2147483648;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_normal() {
  dump("testing: test_eq_i32_f64_min_normal");
  let a0 = -2147483648;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_min() {
  dump("testing: test_eq_i32_f64_min_min");
  let a0 = -2147483648;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_max_max() {
  dump("testing: test_eq_i32_i32_max_max");
  let a0 = 2147483647;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_max_zero() {
  dump("testing: test_eq_i32_i32_max_zero");
  let a0 = 2147483647;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_max_normal() {
  dump("testing: test_eq_i32_i32_max_normal");
  let a0 = 2147483647;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_max_min() {
  dump("testing: test_eq_i32_i32_max_min");
  let a0 = 2147483647;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_zero_max() {
  dump("testing: test_eq_i32_i32_zero_max");
  let a0 = 0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_zero_zero() {
  dump("testing: test_eq_i32_i32_zero_zero");
  let a0 = 0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_zero_normal() {
  dump("testing: test_eq_i32_i32_zero_normal");
  let a0 = 0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_zero_min() {
  dump("testing: test_eq_i32_i32_zero_min");
  let a0 = 0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_normal_max() {
  dump("testing: test_eq_i32_i32_normal_max");
  let a0 = 14;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_normal_zero() {
  dump("testing: test_eq_i32_i32_normal_zero");
  let a0 = 14;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_normal_normal() {
  dump("testing: test_eq_i32_i32_normal_normal");
  let a0 = 14;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_normal_min() {
  dump("testing: test_eq_i32_i32_normal_min");
  let a0 = 14;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_min_max() {
  dump("testing: test_eq_i32_i32_min_max");
  let a0 = -2147483648;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_min_zero() {
  dump("testing: test_eq_i32_i32_min_zero");
  let a0 = -2147483648;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_min_normal() {
  dump("testing: test_eq_i32_i32_min_normal");
  let a0 = -2147483648;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_i32_min_min() {
  dump("testing: test_eq_i32_i32_min_min");
  let a0 = -2147483648;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_max_max() {
  dump("testing: test_eq_i32_string_max_max");
  let a0 = 2147483647;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_max_zero() {
  dump("testing: test_eq_i32_string_max_zero");
  let a0 = 2147483647;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_max_normal() {
  dump("testing: test_eq_i32_string_max_normal");
  let a0 = 2147483647;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_max_min() {
  dump("testing: test_eq_i32_string_max_min");
  let a0 = 2147483647;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_zero_max() {
  dump("testing: test_eq_i32_string_zero_max");
  let a0 = 0;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_zero_zero() {
  dump("testing: test_eq_i32_string_zero_zero");
  let a0 = 0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_zero_normal() {
  dump("testing: test_eq_i32_string_zero_normal");
  let a0 = 0;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_zero_min() {
  dump("testing: test_eq_i32_string_zero_min");
  let a0 = 0;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_normal_max() {
  dump("testing: test_eq_i32_string_normal_max");
  let a0 = 14;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_normal_zero() {
  dump("testing: test_eq_i32_string_normal_zero");
  let a0 = 14;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_normal_normal() {
  dump("testing: test_eq_i32_string_normal_normal");
  let a0 = 14;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_normal_min() {
  dump("testing: test_eq_i32_string_normal_min");
  let a0 = 14;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_min_max() {
  dump("testing: test_eq_i32_string_min_max");
  let a0 = -2147483648;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_min_zero() {
  dump("testing: test_eq_i32_string_min_zero");
  let a0 = -2147483648;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_min_normal() {
  dump("testing: test_eq_i32_string_min_normal");
  let a0 = -2147483648;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_string_min_min() {
  dump("testing: test_eq_i32_string_min_min");
  let a0 = -2147483648;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_max_max() {
  dump("testing: test_eq_i32_bool_max_max");
  let a0 = 2147483647;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_max_zero() {
  dump("testing: test_eq_i32_bool_max_zero");
  let a0 = 2147483647;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_max_normal() {
  dump("testing: test_eq_i32_bool_max_normal");
  let a0 = 2147483647;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_max_min() {
  dump("testing: test_eq_i32_bool_max_min");
  let a0 = 2147483647;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_zero_max() {
  dump("testing: test_eq_i32_bool_zero_max");
  let a0 = 0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_zero_zero() {
  dump("testing: test_eq_i32_bool_zero_zero");
  let a0 = 0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_zero_normal() {
  dump("testing: test_eq_i32_bool_zero_normal");
  let a0 = 0;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_zero_min() {
  dump("testing: test_eq_i32_bool_zero_min");
  let a0 = 0;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_normal_max() {
  dump("testing: test_eq_i32_bool_normal_max");
  let a0 = 14;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_normal_zero() {
  dump("testing: test_eq_i32_bool_normal_zero");
  let a0 = 14;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_normal_normal() {
  dump("testing: test_eq_i32_bool_normal_normal");
  let a0 = 14;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_normal_min() {
  dump("testing: test_eq_i32_bool_normal_min");
  let a0 = 14;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_min_max() {
  dump("testing: test_eq_i32_bool_min_max");
  let a0 = -2147483648;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_min_zero() {
  dump("testing: test_eq_i32_bool_min_zero");
  let a0 = -2147483648;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_min_normal() {
  dump("testing: test_eq_i32_bool_min_normal");
  let a0 = -2147483648;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_bool_min_min() {
  dump("testing: test_eq_i32_bool_min_min");
  let a0 = -2147483648;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_max_max() {
  dump("testing: test_eq_i32_undefined_or_null_max_max");
  let a0 = 2147483647;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_max_zero() {
  dump("testing: test_eq_i32_undefined_or_null_max_zero");
  let a0 = 2147483647;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_max_normal() {
  dump("testing: test_eq_i32_undefined_or_null_max_normal");
  let a0 = 2147483647;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_max_min() {
  dump("testing: test_eq_i32_undefined_or_null_max_min");
  let a0 = 2147483647;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_zero_max() {
  dump("testing: test_eq_i32_undefined_or_null_zero_max");
  let a0 = 0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_zero_zero() {
  dump("testing: test_eq_i32_undefined_or_null_zero_zero");
  let a0 = 0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_zero_normal() {
  dump("testing: test_eq_i32_undefined_or_null_zero_normal");
  let a0 = 0;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_zero_min() {
  dump("testing: test_eq_i32_undefined_or_null_zero_min");
  let a0 = 0;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_normal_max() {
  dump("testing: test_eq_i32_undefined_or_null_normal_max");
  let a0 = 14;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_normal_zero() {
  dump("testing: test_eq_i32_undefined_or_null_normal_zero");
  let a0 = 14;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_normal_normal() {
  dump("testing: test_eq_i32_undefined_or_null_normal_normal");
  let a0 = 14;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_normal_min() {
  dump("testing: test_eq_i32_undefined_or_null_normal_min");
  let a0 = 14;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_min_max() {
  dump("testing: test_eq_i32_undefined_or_null_min_max");
  let a0 = -2147483648;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_min_zero() {
  dump("testing: test_eq_i32_undefined_or_null_min_zero");
  let a0 = -2147483648;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_min_normal() {
  dump("testing: test_eq_i32_undefined_or_null_min_normal");
  let a0 = -2147483648;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_undefined_or_null_min_min() {
  dump("testing: test_eq_i32_undefined_or_null_min_min");
  let a0 = -2147483648;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_max() {
  dump("testing: test_eq_string_f64_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_zero() {
  dump("testing: test_eq_string_f64_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_normal() {
  dump("testing: test_eq_string_f64_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_min() {
  dump("testing: test_eq_string_f64_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_max() {
  dump("testing: test_eq_string_f64_zero_max");
  let a0 = '';
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_zero() {
  dump("testing: test_eq_string_f64_zero_zero");
  let a0 = '';
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_normal() {
  dump("testing: test_eq_string_f64_zero_normal");
  let a0 = '';
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_min() {
  dump("testing: test_eq_string_f64_zero_min");
  let a0 = '';
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_max() {
  dump("testing: test_eq_string_f64_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_zero() {
  dump("testing: test_eq_string_f64_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_normal() {
  dump("testing: test_eq_string_f64_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_min() {
  dump("testing: test_eq_string_f64_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_max() {
  dump("testing: test_eq_string_f64_min_max");
  let a0 = '';
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_zero() {
  dump("testing: test_eq_string_f64_min_zero");
  let a0 = '';
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_normal() {
  dump("testing: test_eq_string_f64_min_normal");
  let a0 = '';
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_min() {
  dump("testing: test_eq_string_f64_min_min");
  let a0 = '';
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_max_max() {
  dump("testing: test_eq_string_i32_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_max_zero() {
  dump("testing: test_eq_string_i32_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_max_normal() {
  dump("testing: test_eq_string_i32_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_max_min() {
  dump("testing: test_eq_string_i32_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_zero_max() {
  dump("testing: test_eq_string_i32_zero_max");
  let a0 = '';
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_zero_zero() {
  dump("testing: test_eq_string_i32_zero_zero");
  let a0 = '';
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_zero_normal() {
  dump("testing: test_eq_string_i32_zero_normal");
  let a0 = '';
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_zero_min() {
  dump("testing: test_eq_string_i32_zero_min");
  let a0 = '';
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_normal_max() {
  dump("testing: test_eq_string_i32_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_normal_zero() {
  dump("testing: test_eq_string_i32_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_normal_normal() {
  dump("testing: test_eq_string_i32_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_normal_min() {
  dump("testing: test_eq_string_i32_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_min_max() {
  dump("testing: test_eq_string_i32_min_max");
  let a0 = '';
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_min_zero() {
  dump("testing: test_eq_string_i32_min_zero");
  let a0 = '';
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_min_normal() {
  dump("testing: test_eq_string_i32_min_normal");
  let a0 = '';
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_i32_min_min() {
  dump("testing: test_eq_string_i32_min_min");
  let a0 = '';
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_max_max() {
  dump("testing: test_eq_string_string_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_max_zero() {
  dump("testing: test_eq_string_string_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_max_normal() {
  dump("testing: test_eq_string_string_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_max_min() {
  dump("testing: test_eq_string_string_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_zero_max() {
  dump("testing: test_eq_string_string_zero_max");
  let a0 = '';
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_zero_zero() {
  dump("testing: test_eq_string_string_zero_zero");
  let a0 = '';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_zero_normal() {
  dump("testing: test_eq_string_string_zero_normal");
  let a0 = '';
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_zero_min() {
  dump("testing: test_eq_string_string_zero_min");
  let a0 = '';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_normal_max() {
  dump("testing: test_eq_string_string_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_normal_zero() {
  dump("testing: test_eq_string_string_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_normal_normal() {
  dump("testing: test_eq_string_string_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_normal_min() {
  dump("testing: test_eq_string_string_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_min_max() {
  dump("testing: test_eq_string_string_min_max");
  let a0 = '';
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_min_zero() {
  dump("testing: test_eq_string_string_min_zero");
  let a0 = '';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_min_normal() {
  dump("testing: test_eq_string_string_min_normal");
  let a0 = '';
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_string_min_min() {
  dump("testing: test_eq_string_string_min_min");
  let a0 = '';
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_max_max() {
  dump("testing: test_eq_string_bool_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_max_zero() {
  dump("testing: test_eq_string_bool_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_max_normal() {
  dump("testing: test_eq_string_bool_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_max_min() {
  dump("testing: test_eq_string_bool_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_zero_max() {
  dump("testing: test_eq_string_bool_zero_max");
  let a0 = '';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_zero_zero() {
  dump("testing: test_eq_string_bool_zero_zero");
  let a0 = '';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_zero_normal() {
  dump("testing: test_eq_string_bool_zero_normal");
  let a0 = '';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_zero_min() {
  dump("testing: test_eq_string_bool_zero_min");
  let a0 = '';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_normal_max() {
  dump("testing: test_eq_string_bool_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_normal_zero() {
  dump("testing: test_eq_string_bool_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_normal_normal() {
  dump("testing: test_eq_string_bool_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_normal_min() {
  dump("testing: test_eq_string_bool_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_min_max() {
  dump("testing: test_eq_string_bool_min_max");
  let a0 = '';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_min_zero() {
  dump("testing: test_eq_string_bool_min_zero");
  let a0 = '';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_min_normal() {
  dump("testing: test_eq_string_bool_min_normal");
  let a0 = '';
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_bool_min_min() {
  dump("testing: test_eq_string_bool_min_min");
  let a0 = '';
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_max_max() {
  dump("testing: test_eq_string_undefined_or_null_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_max_zero() {
  dump("testing: test_eq_string_undefined_or_null_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_max_normal() {
  dump("testing: test_eq_string_undefined_or_null_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_max_min() {
  dump("testing: test_eq_string_undefined_or_null_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_zero_max() {
  dump("testing: test_eq_string_undefined_or_null_zero_max");
  let a0 = '';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_zero_zero() {
  dump("testing: test_eq_string_undefined_or_null_zero_zero");
  let a0 = '';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_zero_normal() {
  dump("testing: test_eq_string_undefined_or_null_zero_normal");
  let a0 = '';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_zero_min() {
  dump("testing: test_eq_string_undefined_or_null_zero_min");
  let a0 = '';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_normal_max() {
  dump("testing: test_eq_string_undefined_or_null_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_normal_zero() {
  dump("testing: test_eq_string_undefined_or_null_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_normal_normal() {
  dump("testing: test_eq_string_undefined_or_null_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_normal_min() {
  dump("testing: test_eq_string_undefined_or_null_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_min_max() {
  dump("testing: test_eq_string_undefined_or_null_min_max");
  let a0 = '';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_min_zero() {
  dump("testing: test_eq_string_undefined_or_null_min_zero");
  let a0 = '';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_min_normal() {
  dump("testing: test_eq_string_undefined_or_null_min_normal");
  let a0 = '';
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_undefined_or_null_min_min() {
  dump("testing: test_eq_string_undefined_or_null_min_min");
  let a0 = '';
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_max() {
  dump("testing: test_eq_bool_f64_max_max");
  let a0 = true;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_zero() {
  dump("testing: test_eq_bool_f64_max_zero");
  let a0 = true;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_normal() {
  dump("testing: test_eq_bool_f64_max_normal");
  let a0 = true;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_min() {
  dump("testing: test_eq_bool_f64_max_min");
  let a0 = true;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_max() {
  dump("testing: test_eq_bool_f64_zero_max");
  let a0 = false;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_zero() {
  dump("testing: test_eq_bool_f64_zero_zero");
  let a0 = false;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_normal() {
  dump("testing: test_eq_bool_f64_zero_normal");
  let a0 = false;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_min() {
  dump("testing: test_eq_bool_f64_zero_min");
  let a0 = false;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_max() {
  dump("testing: test_eq_bool_f64_normal_max");
  let a0 = true;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_zero() {
  dump("testing: test_eq_bool_f64_normal_zero");
  let a0 = true;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_normal() {
  dump("testing: test_eq_bool_f64_normal_normal");
  let a0 = true;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_min() {
  dump("testing: test_eq_bool_f64_normal_min");
  let a0 = true;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_max() {
  dump("testing: test_eq_bool_f64_min_max");
  let a0 = false;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_zero() {
  dump("testing: test_eq_bool_f64_min_zero");
  let a0 = false;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_normal() {
  dump("testing: test_eq_bool_f64_min_normal");
  let a0 = false;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_min() {
  dump("testing: test_eq_bool_f64_min_min");
  let a0 = false;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_max_max() {
  dump("testing: test_eq_bool_i32_max_max");
  let a0 = true;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_max_zero() {
  dump("testing: test_eq_bool_i32_max_zero");
  let a0 = true;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_max_normal() {
  dump("testing: test_eq_bool_i32_max_normal");
  let a0 = true;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_max_min() {
  dump("testing: test_eq_bool_i32_max_min");
  let a0 = true;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_zero_max() {
  dump("testing: test_eq_bool_i32_zero_max");
  let a0 = false;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_zero_zero() {
  dump("testing: test_eq_bool_i32_zero_zero");
  let a0 = false;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_zero_normal() {
  dump("testing: test_eq_bool_i32_zero_normal");
  let a0 = false;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_zero_min() {
  dump("testing: test_eq_bool_i32_zero_min");
  let a0 = false;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_normal_max() {
  dump("testing: test_eq_bool_i32_normal_max");
  let a0 = true;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_normal_zero() {
  dump("testing: test_eq_bool_i32_normal_zero");
  let a0 = true;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_normal_normal() {
  dump("testing: test_eq_bool_i32_normal_normal");
  let a0 = true;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_normal_min() {
  dump("testing: test_eq_bool_i32_normal_min");
  let a0 = true;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_min_max() {
  dump("testing: test_eq_bool_i32_min_max");
  let a0 = false;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_min_zero() {
  dump("testing: test_eq_bool_i32_min_zero");
  let a0 = false;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_min_normal() {
  dump("testing: test_eq_bool_i32_min_normal");
  let a0 = false;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_i32_min_min() {
  dump("testing: test_eq_bool_i32_min_min");
  let a0 = false;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_max_max() {
  dump("testing: test_eq_bool_string_max_max");
  let a0 = true;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_max_zero() {
  dump("testing: test_eq_bool_string_max_zero");
  let a0 = true;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_max_normal() {
  dump("testing: test_eq_bool_string_max_normal");
  let a0 = true;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_max_min() {
  dump("testing: test_eq_bool_string_max_min");
  let a0 = true;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_zero_max() {
  dump("testing: test_eq_bool_string_zero_max");
  let a0 = false;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_zero_zero() {
  dump("testing: test_eq_bool_string_zero_zero");
  let a0 = false;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_zero_normal() {
  dump("testing: test_eq_bool_string_zero_normal");
  let a0 = false;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_zero_min() {
  dump("testing: test_eq_bool_string_zero_min");
  let a0 = false;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_normal_max() {
  dump("testing: test_eq_bool_string_normal_max");
  let a0 = true;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_normal_zero() {
  dump("testing: test_eq_bool_string_normal_zero");
  let a0 = true;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_normal_normal() {
  dump("testing: test_eq_bool_string_normal_normal");
  let a0 = true;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_normal_min() {
  dump("testing: test_eq_bool_string_normal_min");
  let a0 = true;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_min_max() {
  dump("testing: test_eq_bool_string_min_max");
  let a0 = false;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_min_zero() {
  dump("testing: test_eq_bool_string_min_zero");
  let a0 = false;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_min_normal() {
  dump("testing: test_eq_bool_string_min_normal");
  let a0 = false;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_string_min_min() {
  dump("testing: test_eq_bool_string_min_min");
  let a0 = false;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_max_max() {
  dump("testing: test_eq_bool_bool_max_max");
  let a0 = true;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_max_zero() {
  dump("testing: test_eq_bool_bool_max_zero");
  let a0 = true;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_max_normal() {
  dump("testing: test_eq_bool_bool_max_normal");
  let a0 = true;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_max_min() {
  dump("testing: test_eq_bool_bool_max_min");
  let a0 = true;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_zero_max() {
  dump("testing: test_eq_bool_bool_zero_max");
  let a0 = false;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_zero_zero() {
  dump("testing: test_eq_bool_bool_zero_zero");
  let a0 = false;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_zero_normal() {
  dump("testing: test_eq_bool_bool_zero_normal");
  let a0 = false;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_zero_min() {
  dump("testing: test_eq_bool_bool_zero_min");
  let a0 = false;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_normal_max() {
  dump("testing: test_eq_bool_bool_normal_max");
  let a0 = true;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_normal_zero() {
  dump("testing: test_eq_bool_bool_normal_zero");
  let a0 = true;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_normal_normal() {
  dump("testing: test_eq_bool_bool_normal_normal");
  let a0 = true;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_normal_min() {
  dump("testing: test_eq_bool_bool_normal_min");
  let a0 = true;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_min_max() {
  dump("testing: test_eq_bool_bool_min_max");
  let a0 = false;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_min_zero() {
  dump("testing: test_eq_bool_bool_min_zero");
  let a0 = false;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_min_normal() {
  dump("testing: test_eq_bool_bool_min_normal");
  let a0 = false;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_bool_min_min() {
  dump("testing: test_eq_bool_bool_min_min");
  let a0 = false;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_max_max() {
  dump("testing: test_eq_bool_undefined_or_null_max_max");
  let a0 = true;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_max_zero() {
  dump("testing: test_eq_bool_undefined_or_null_max_zero");
  let a0 = true;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_max_normal() {
  dump("testing: test_eq_bool_undefined_or_null_max_normal");
  let a0 = true;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_max_min() {
  dump("testing: test_eq_bool_undefined_or_null_max_min");
  let a0 = true;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_zero_max() {
  dump("testing: test_eq_bool_undefined_or_null_zero_max");
  let a0 = false;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_zero_zero() {
  dump("testing: test_eq_bool_undefined_or_null_zero_zero");
  let a0 = false;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_zero_normal() {
  dump("testing: test_eq_bool_undefined_or_null_zero_normal");
  let a0 = false;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_zero_min() {
  dump("testing: test_eq_bool_undefined_or_null_zero_min");
  let a0 = false;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_normal_max() {
  dump("testing: test_eq_bool_undefined_or_null_normal_max");
  let a0 = true;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_normal_zero() {
  dump("testing: test_eq_bool_undefined_or_null_normal_zero");
  let a0 = true;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_normal_normal() {
  dump("testing: test_eq_bool_undefined_or_null_normal_normal");
  let a0 = true;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_normal_min() {
  dump("testing: test_eq_bool_undefined_or_null_normal_min");
  let a0 = true;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_min_max() {
  dump("testing: test_eq_bool_undefined_or_null_min_max");
  let a0 = false;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_min_zero() {
  dump("testing: test_eq_bool_undefined_or_null_min_zero");
  let a0 = false;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_min_normal() {
  dump("testing: test_eq_bool_undefined_or_null_min_normal");
  let a0 = false;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_undefined_or_null_min_min() {
  dump("testing: test_eq_bool_undefined_or_null_min_min");
  let a0 = false;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_max() {
  dump("testing: test_eq_undefined_or_null_f64_max_max");
  let a0 = null;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_zero() {
  dump("testing: test_eq_undefined_or_null_f64_max_zero");
  let a0 = null;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_normal() {
  dump("testing: test_eq_undefined_or_null_f64_max_normal");
  let a0 = null;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_min() {
  dump("testing: test_eq_undefined_or_null_f64_max_min");
  let a0 = null;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_max() {
  dump("testing: test_eq_undefined_or_null_f64_zero_max");
  let a0 = undefined;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_zero() {
  dump("testing: test_eq_undefined_or_null_f64_zero_zero");
  let a0 = undefined;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_normal() {
  dump("testing: test_eq_undefined_or_null_f64_zero_normal");
  let a0 = undefined;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_min() {
  dump("testing: test_eq_undefined_or_null_f64_zero_min");
  let a0 = undefined;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_max() {
  dump("testing: test_eq_undefined_or_null_f64_normal_max");
  let a0 = null;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_zero() {
  dump("testing: test_eq_undefined_or_null_f64_normal_zero");
  let a0 = null;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_normal() {
  dump("testing: test_eq_undefined_or_null_f64_normal_normal");
  let a0 = null;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_min() {
  dump("testing: test_eq_undefined_or_null_f64_normal_min");
  let a0 = null;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_max() {
  dump("testing: test_eq_undefined_or_null_f64_min_max");
  let a0 = undefined;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_zero() {
  dump("testing: test_eq_undefined_or_null_f64_min_zero");
  let a0 = undefined;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_normal() {
  dump("testing: test_eq_undefined_or_null_f64_min_normal");
  let a0 = undefined;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_min() {
  dump("testing: test_eq_undefined_or_null_f64_min_min");
  let a0 = undefined;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_max_max() {
  dump("testing: test_eq_undefined_or_null_i32_max_max");
  let a0 = null;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_max_zero() {
  dump("testing: test_eq_undefined_or_null_i32_max_zero");
  let a0 = null;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_max_normal() {
  dump("testing: test_eq_undefined_or_null_i32_max_normal");
  let a0 = null;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_max_min() {
  dump("testing: test_eq_undefined_or_null_i32_max_min");
  let a0 = null;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_zero_max() {
  dump("testing: test_eq_undefined_or_null_i32_zero_max");
  let a0 = undefined;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_zero_zero() {
  dump("testing: test_eq_undefined_or_null_i32_zero_zero");
  let a0 = undefined;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_zero_normal() {
  dump("testing: test_eq_undefined_or_null_i32_zero_normal");
  let a0 = undefined;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_zero_min() {
  dump("testing: test_eq_undefined_or_null_i32_zero_min");
  let a0 = undefined;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_normal_max() {
  dump("testing: test_eq_undefined_or_null_i32_normal_max");
  let a0 = null;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_normal_zero() {
  dump("testing: test_eq_undefined_or_null_i32_normal_zero");
  let a0 = null;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_normal_normal() {
  dump("testing: test_eq_undefined_or_null_i32_normal_normal");
  let a0 = null;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_normal_min() {
  dump("testing: test_eq_undefined_or_null_i32_normal_min");
  let a0 = null;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_min_max() {
  dump("testing: test_eq_undefined_or_null_i32_min_max");
  let a0 = undefined;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_min_zero() {
  dump("testing: test_eq_undefined_or_null_i32_min_zero");
  let a0 = undefined;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_min_normal() {
  dump("testing: test_eq_undefined_or_null_i32_min_normal");
  let a0 = undefined;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_i32_min_min() {
  dump("testing: test_eq_undefined_or_null_i32_min_min");
  let a0 = undefined;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_max_max() {
  dump("testing: test_eq_undefined_or_null_string_max_max");
  let a0 = null;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_max_zero() {
  dump("testing: test_eq_undefined_or_null_string_max_zero");
  let a0 = null;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_max_normal() {
  dump("testing: test_eq_undefined_or_null_string_max_normal");
  let a0 = null;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_max_min() {
  dump("testing: test_eq_undefined_or_null_string_max_min");
  let a0 = null;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_zero_max() {
  dump("testing: test_eq_undefined_or_null_string_zero_max");
  let a0 = undefined;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_zero_zero() {
  dump("testing: test_eq_undefined_or_null_string_zero_zero");
  let a0 = undefined;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_zero_normal() {
  dump("testing: test_eq_undefined_or_null_string_zero_normal");
  let a0 = undefined;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_zero_min() {
  dump("testing: test_eq_undefined_or_null_string_zero_min");
  let a0 = undefined;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_normal_max() {
  dump("testing: test_eq_undefined_or_null_string_normal_max");
  let a0 = null;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_normal_zero() {
  dump("testing: test_eq_undefined_or_null_string_normal_zero");
  let a0 = null;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_normal_normal() {
  dump("testing: test_eq_undefined_or_null_string_normal_normal");
  let a0 = null;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_normal_min() {
  dump("testing: test_eq_undefined_or_null_string_normal_min");
  let a0 = null;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_min_max() {
  dump("testing: test_eq_undefined_or_null_string_min_max");
  let a0 = undefined;
  let a1 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_min_zero() {
  dump("testing: test_eq_undefined_or_null_string_min_zero");
  let a0 = undefined;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_min_normal() {
  dump("testing: test_eq_undefined_or_null_string_min_normal");
  let a0 = undefined;
  let a1 = 'aaaabbccc';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_string_min_min() {
  dump("testing: test_eq_undefined_or_null_string_min_min");
  let a0 = undefined;
  let a1 = '';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_max_max() {
  dump("testing: test_eq_undefined_or_null_bool_max_max");
  let a0 = null;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_max_zero() {
  dump("testing: test_eq_undefined_or_null_bool_max_zero");
  let a0 = null;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_max_normal() {
  dump("testing: test_eq_undefined_or_null_bool_max_normal");
  let a0 = null;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_max_min() {
  dump("testing: test_eq_undefined_or_null_bool_max_min");
  let a0 = null;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_zero_max() {
  dump("testing: test_eq_undefined_or_null_bool_zero_max");
  let a0 = undefined;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_zero_zero() {
  dump("testing: test_eq_undefined_or_null_bool_zero_zero");
  let a0 = undefined;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_zero_normal() {
  dump("testing: test_eq_undefined_or_null_bool_zero_normal");
  let a0 = undefined;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_zero_min() {
  dump("testing: test_eq_undefined_or_null_bool_zero_min");
  let a0 = undefined;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_normal_max() {
  dump("testing: test_eq_undefined_or_null_bool_normal_max");
  let a0 = null;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_normal_zero() {
  dump("testing: test_eq_undefined_or_null_bool_normal_zero");
  let a0 = null;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_normal_normal() {
  dump("testing: test_eq_undefined_or_null_bool_normal_normal");
  let a0 = null;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_normal_min() {
  dump("testing: test_eq_undefined_or_null_bool_normal_min");
  let a0 = null;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_min_max() {
  dump("testing: test_eq_undefined_or_null_bool_min_max");
  let a0 = undefined;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_min_zero() {
  dump("testing: test_eq_undefined_or_null_bool_min_zero");
  let a0 = undefined;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_min_normal() {
  dump("testing: test_eq_undefined_or_null_bool_min_normal");
  let a0 = undefined;
  let a1 = true;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_bool_min_min() {
  dump("testing: test_eq_undefined_or_null_bool_min_min");
  let a0 = undefined;
  let a1 = false;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_max_max() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_max_max");
  let a0 = null;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_max_zero() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_max_zero");
  let a0 = null;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_max_normal() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_max_normal");
  let a0 = null;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_max_min() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_max_min");
  let a0 = null;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_zero_max() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_zero_max");
  let a0 = undefined;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_zero_zero() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_zero_zero");
  let a0 = undefined;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_zero_normal() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_zero_normal");
  let a0 = undefined;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_zero_min() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_zero_min");
  let a0 = undefined;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_normal_max() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_normal_max");
  let a0 = null;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_normal_zero() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_normal_zero");
  let a0 = null;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_normal_normal() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_normal_normal");
  let a0 = null;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_normal_min() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_normal_min");
  let a0 = null;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_min_max() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_min_max");
  let a0 = undefined;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_min_zero() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_min_zero");
  let a0 = undefined;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_min_normal() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_min_normal");
  let a0 = undefined;
  let a1 = null;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_undefined_or_null_min_min() {
  dump("testing: test_eq_undefined_or_null_undefined_or_null_min_min");
  let a0 = undefined;
  let a1 = undefined;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}
  test_eq_f64_f64_max_max();
  test_eq_f64_f64_max_zero();
  test_eq_f64_f64_max_normal();
  test_eq_f64_f64_max_min();
  test_eq_f64_f64_zero_max();
  test_eq_f64_f64_zero_zero();
  test_eq_f64_f64_zero_normal();
  test_eq_f64_f64_zero_min();
  test_eq_f64_f64_normal_max();
  test_eq_f64_f64_normal_zero();
  test_eq_f64_f64_normal_normal();
  test_eq_f64_f64_normal_min();
  test_eq_f64_f64_min_max();
  test_eq_f64_f64_min_zero();
  test_eq_f64_f64_min_normal();
  test_eq_f64_f64_min_min();
  test_eq_f64_i32_max_max();
  test_eq_f64_i32_max_zero();
  test_eq_f64_i32_max_normal();
  test_eq_f64_i32_max_min();
  test_eq_f64_i32_zero_max();
  test_eq_f64_i32_zero_zero();
  test_eq_f64_i32_zero_normal();
  test_eq_f64_i32_zero_min();
  test_eq_f64_i32_normal_max();
  test_eq_f64_i32_normal_zero();
  test_eq_f64_i32_normal_normal();
  test_eq_f64_i32_normal_min();
  test_eq_f64_i32_min_max();
  test_eq_f64_i32_min_zero();
  test_eq_f64_i32_min_normal();
  test_eq_f64_i32_min_min();
  test_eq_f64_string_max_max();
  test_eq_f64_string_max_zero();
  test_eq_f64_string_max_normal();
  test_eq_f64_string_max_min();
  test_eq_f64_string_zero_max();
  test_eq_f64_string_zero_zero();
  test_eq_f64_string_zero_normal();
  test_eq_f64_string_zero_min();
  test_eq_f64_string_normal_max();
  test_eq_f64_string_normal_zero();
  test_eq_f64_string_normal_normal();
  test_eq_f64_string_normal_min();
  test_eq_f64_string_min_max();
  test_eq_f64_string_min_zero();
  test_eq_f64_string_min_normal();
  test_eq_f64_string_min_min();
  test_eq_f64_bool_max_max();
  test_eq_f64_bool_max_zero();
  test_eq_f64_bool_max_normal();
  test_eq_f64_bool_max_min();
  test_eq_f64_bool_zero_max();
  test_eq_f64_bool_zero_zero();
  test_eq_f64_bool_zero_normal();
  test_eq_f64_bool_zero_min();
  test_eq_f64_bool_normal_max();
  test_eq_f64_bool_normal_zero();
  test_eq_f64_bool_normal_normal();
  test_eq_f64_bool_normal_min();
  test_eq_f64_bool_min_max();
  test_eq_f64_bool_min_zero();
  test_eq_f64_bool_min_normal();
  test_eq_f64_bool_min_min();
  test_eq_f64_undefined_or_null_max_max();
  test_eq_f64_undefined_or_null_max_zero();
  test_eq_f64_undefined_or_null_max_normal();
  test_eq_f64_undefined_or_null_max_min();
  test_eq_f64_undefined_or_null_zero_max();
  test_eq_f64_undefined_or_null_zero_zero();
  test_eq_f64_undefined_or_null_zero_normal();
  test_eq_f64_undefined_or_null_zero_min();
  test_eq_f64_undefined_or_null_normal_max();
  test_eq_f64_undefined_or_null_normal_zero();
  test_eq_f64_undefined_or_null_normal_normal();
  test_eq_f64_undefined_or_null_normal_min();
  test_eq_f64_undefined_or_null_min_max();
  test_eq_f64_undefined_or_null_min_zero();
  test_eq_f64_undefined_or_null_min_normal();
  test_eq_f64_undefined_or_null_min_min();
  test_eq_i32_f64_max_max();
  test_eq_i32_f64_max_zero();
  test_eq_i32_f64_max_normal();
  test_eq_i32_f64_max_min();
  test_eq_i32_f64_zero_max();
  test_eq_i32_f64_zero_zero();
  test_eq_i32_f64_zero_normal();
  test_eq_i32_f64_zero_min();
  test_eq_i32_f64_normal_max();
  test_eq_i32_f64_normal_zero();
  test_eq_i32_f64_normal_normal();
  test_eq_i32_f64_normal_min();
  test_eq_i32_f64_min_max();
  test_eq_i32_f64_min_zero();
  test_eq_i32_f64_min_normal();
  test_eq_i32_f64_min_min();
  test_eq_i32_i32_max_max();
  test_eq_i32_i32_max_zero();
  test_eq_i32_i32_max_normal();
  test_eq_i32_i32_max_min();
  test_eq_i32_i32_zero_max();
  test_eq_i32_i32_zero_zero();
  test_eq_i32_i32_zero_normal();
  test_eq_i32_i32_zero_min();
  test_eq_i32_i32_normal_max();
  test_eq_i32_i32_normal_zero();
  test_eq_i32_i32_normal_normal();
  test_eq_i32_i32_normal_min();
  test_eq_i32_i32_min_max();
  test_eq_i32_i32_min_zero();
  test_eq_i32_i32_min_normal();
  test_eq_i32_i32_min_min();
  test_eq_i32_string_max_max();
  test_eq_i32_string_max_zero();
  test_eq_i32_string_max_normal();
  test_eq_i32_string_max_min();
  test_eq_i32_string_zero_max();
  test_eq_i32_string_zero_zero();
  test_eq_i32_string_zero_normal();
  test_eq_i32_string_zero_min();
  test_eq_i32_string_normal_max();
  test_eq_i32_string_normal_zero();
  test_eq_i32_string_normal_normal();
  test_eq_i32_string_normal_min();
  test_eq_i32_string_min_max();
  test_eq_i32_string_min_zero();
  test_eq_i32_string_min_normal();
  test_eq_i32_string_min_min();
  test_eq_i32_bool_max_max();
  test_eq_i32_bool_max_zero();
  test_eq_i32_bool_max_normal();
  test_eq_i32_bool_max_min();
  test_eq_i32_bool_zero_max();
  test_eq_i32_bool_zero_zero();
  test_eq_i32_bool_zero_normal();
  test_eq_i32_bool_zero_min();
  test_eq_i32_bool_normal_max();
  test_eq_i32_bool_normal_zero();
  test_eq_i32_bool_normal_normal();
  test_eq_i32_bool_normal_min();
  test_eq_i32_bool_min_max();
  test_eq_i32_bool_min_zero();
  test_eq_i32_bool_min_normal();
  test_eq_i32_bool_min_min();
  test_eq_i32_undefined_or_null_max_max();
  test_eq_i32_undefined_or_null_max_zero();
  test_eq_i32_undefined_or_null_max_normal();
  test_eq_i32_undefined_or_null_max_min();
  test_eq_i32_undefined_or_null_zero_max();
  test_eq_i32_undefined_or_null_zero_zero();
  test_eq_i32_undefined_or_null_zero_normal();
  test_eq_i32_undefined_or_null_zero_min();
  test_eq_i32_undefined_or_null_normal_max();
  test_eq_i32_undefined_or_null_normal_zero();
  test_eq_i32_undefined_or_null_normal_normal();
  test_eq_i32_undefined_or_null_normal_min();
  test_eq_i32_undefined_or_null_min_max();
  test_eq_i32_undefined_or_null_min_zero();
  test_eq_i32_undefined_or_null_min_normal();
  test_eq_i32_undefined_or_null_min_min();
  test_eq_string_f64_max_max();
  test_eq_string_f64_max_zero();
  test_eq_string_f64_max_normal();
  test_eq_string_f64_max_min();
  test_eq_string_f64_zero_max();
  test_eq_string_f64_zero_zero();
  test_eq_string_f64_zero_normal();
  test_eq_string_f64_zero_min();
  test_eq_string_f64_normal_max();
  test_eq_string_f64_normal_zero();
  test_eq_string_f64_normal_normal();
  test_eq_string_f64_normal_min();
  test_eq_string_f64_min_max();
  test_eq_string_f64_min_zero();
  test_eq_string_f64_min_normal();
  test_eq_string_f64_min_min();
  test_eq_string_i32_max_max();
  test_eq_string_i32_max_zero();
  test_eq_string_i32_max_normal();
  test_eq_string_i32_max_min();
  test_eq_string_i32_zero_max();
  test_eq_string_i32_zero_zero();
  test_eq_string_i32_zero_normal();
  test_eq_string_i32_zero_min();
  test_eq_string_i32_normal_max();
  test_eq_string_i32_normal_zero();
  test_eq_string_i32_normal_normal();
  test_eq_string_i32_normal_min();
  test_eq_string_i32_min_max();
  test_eq_string_i32_min_zero();
  test_eq_string_i32_min_normal();
  test_eq_string_i32_min_min();
  test_eq_string_string_max_max();
  test_eq_string_string_max_zero();
  test_eq_string_string_max_normal();
  test_eq_string_string_max_min();
  test_eq_string_string_zero_max();
  test_eq_string_string_zero_zero();
  test_eq_string_string_zero_normal();
  test_eq_string_string_zero_min();
  test_eq_string_string_normal_max();
  test_eq_string_string_normal_zero();
  test_eq_string_string_normal_normal();
  test_eq_string_string_normal_min();
  test_eq_string_string_min_max();
  test_eq_string_string_min_zero();
  test_eq_string_string_min_normal();
  test_eq_string_string_min_min();
  test_eq_string_bool_max_max();
  test_eq_string_bool_max_zero();
  test_eq_string_bool_max_normal();
  test_eq_string_bool_max_min();
  test_eq_string_bool_zero_max();
  test_eq_string_bool_zero_zero();
  test_eq_string_bool_zero_normal();
  test_eq_string_bool_zero_min();
  test_eq_string_bool_normal_max();
  test_eq_string_bool_normal_zero();
  test_eq_string_bool_normal_normal();
  test_eq_string_bool_normal_min();
  test_eq_string_bool_min_max();
  test_eq_string_bool_min_zero();
  test_eq_string_bool_min_normal();
  test_eq_string_bool_min_min();
  test_eq_string_undefined_or_null_max_max();
  test_eq_string_undefined_or_null_max_zero();
  test_eq_string_undefined_or_null_max_normal();
  test_eq_string_undefined_or_null_max_min();
  test_eq_string_undefined_or_null_zero_max();
  test_eq_string_undefined_or_null_zero_zero();
  test_eq_string_undefined_or_null_zero_normal();
  test_eq_string_undefined_or_null_zero_min();
  test_eq_string_undefined_or_null_normal_max();
  test_eq_string_undefined_or_null_normal_zero();
  test_eq_string_undefined_or_null_normal_normal();
  test_eq_string_undefined_or_null_normal_min();
  test_eq_string_undefined_or_null_min_max();
  test_eq_string_undefined_or_null_min_zero();
  test_eq_string_undefined_or_null_min_normal();
  test_eq_string_undefined_or_null_min_min();
  test_eq_bool_f64_max_max();
  test_eq_bool_f64_max_zero();
  test_eq_bool_f64_max_normal();
  test_eq_bool_f64_max_min();
  test_eq_bool_f64_zero_max();
  test_eq_bool_f64_zero_zero();
  test_eq_bool_f64_zero_normal();
  test_eq_bool_f64_zero_min();
  test_eq_bool_f64_normal_max();
  test_eq_bool_f64_normal_zero();
  test_eq_bool_f64_normal_normal();
  test_eq_bool_f64_normal_min();
  test_eq_bool_f64_min_max();
  test_eq_bool_f64_min_zero();
  test_eq_bool_f64_min_normal();
  test_eq_bool_f64_min_min();
  test_eq_bool_i32_max_max();
  test_eq_bool_i32_max_zero();
  test_eq_bool_i32_max_normal();
  test_eq_bool_i32_max_min();
  test_eq_bool_i32_zero_max();
  test_eq_bool_i32_zero_zero();
  test_eq_bool_i32_zero_normal();
  test_eq_bool_i32_zero_min();
  test_eq_bool_i32_normal_max();
  test_eq_bool_i32_normal_zero();
  test_eq_bool_i32_normal_normal();
  test_eq_bool_i32_normal_min();
  test_eq_bool_i32_min_max();
  test_eq_bool_i32_min_zero();
  test_eq_bool_i32_min_normal();
  test_eq_bool_i32_min_min();
  test_eq_bool_string_max_max();
  test_eq_bool_string_max_zero();
  test_eq_bool_string_max_normal();
  test_eq_bool_string_max_min();
  test_eq_bool_string_zero_max();
  test_eq_bool_string_zero_zero();
  test_eq_bool_string_zero_normal();
  test_eq_bool_string_zero_min();
  test_eq_bool_string_normal_max();
  test_eq_bool_string_normal_zero();
  test_eq_bool_string_normal_normal();
  test_eq_bool_string_normal_min();
  test_eq_bool_string_min_max();
  test_eq_bool_string_min_zero();
  test_eq_bool_string_min_normal();
  test_eq_bool_string_min_min();
  test_eq_bool_bool_max_max();
  test_eq_bool_bool_max_zero();
  test_eq_bool_bool_max_normal();
  test_eq_bool_bool_max_min();
  test_eq_bool_bool_zero_max();
  test_eq_bool_bool_zero_zero();
  test_eq_bool_bool_zero_normal();
  test_eq_bool_bool_zero_min();
  test_eq_bool_bool_normal_max();
  test_eq_bool_bool_normal_zero();
  test_eq_bool_bool_normal_normal();
  test_eq_bool_bool_normal_min();
  test_eq_bool_bool_min_max();
  test_eq_bool_bool_min_zero();
  test_eq_bool_bool_min_normal();
  test_eq_bool_bool_min_min();
  test_eq_bool_undefined_or_null_max_max();
  test_eq_bool_undefined_or_null_max_zero();
  test_eq_bool_undefined_or_null_max_normal();
  test_eq_bool_undefined_or_null_max_min();
  test_eq_bool_undefined_or_null_zero_max();
  test_eq_bool_undefined_or_null_zero_zero();
  test_eq_bool_undefined_or_null_zero_normal();
  test_eq_bool_undefined_or_null_zero_min();
  test_eq_bool_undefined_or_null_normal_max();
  test_eq_bool_undefined_or_null_normal_zero();
  test_eq_bool_undefined_or_null_normal_normal();
  test_eq_bool_undefined_or_null_normal_min();
  test_eq_bool_undefined_or_null_min_max();
  test_eq_bool_undefined_or_null_min_zero();
  test_eq_bool_undefined_or_null_min_normal();
  test_eq_bool_undefined_or_null_min_min();
  test_eq_undefined_or_null_f64_max_max();
  test_eq_undefined_or_null_f64_max_zero();
  test_eq_undefined_or_null_f64_max_normal();
  test_eq_undefined_or_null_f64_max_min();
  test_eq_undefined_or_null_f64_zero_max();
  test_eq_undefined_or_null_f64_zero_zero();
  test_eq_undefined_or_null_f64_zero_normal();
  test_eq_undefined_or_null_f64_zero_min();
  test_eq_undefined_or_null_f64_normal_max();
  test_eq_undefined_or_null_f64_normal_zero();
  test_eq_undefined_or_null_f64_normal_normal();
  test_eq_undefined_or_null_f64_normal_min();
  test_eq_undefined_or_null_f64_min_max();
  test_eq_undefined_or_null_f64_min_zero();
  test_eq_undefined_or_null_f64_min_normal();
  test_eq_undefined_or_null_f64_min_min();
  test_eq_undefined_or_null_i32_max_max();
  test_eq_undefined_or_null_i32_max_zero();
  test_eq_undefined_or_null_i32_max_normal();
  test_eq_undefined_or_null_i32_max_min();
  test_eq_undefined_or_null_i32_zero_max();
  test_eq_undefined_or_null_i32_zero_zero();
  test_eq_undefined_or_null_i32_zero_normal();
  test_eq_undefined_or_null_i32_zero_min();
  test_eq_undefined_or_null_i32_normal_max();
  test_eq_undefined_or_null_i32_normal_zero();
  test_eq_undefined_or_null_i32_normal_normal();
  test_eq_undefined_or_null_i32_normal_min();
  test_eq_undefined_or_null_i32_min_max();
  test_eq_undefined_or_null_i32_min_zero();
  test_eq_undefined_or_null_i32_min_normal();
  test_eq_undefined_or_null_i32_min_min();
  test_eq_undefined_or_null_string_max_max();
  test_eq_undefined_or_null_string_max_zero();
  test_eq_undefined_or_null_string_max_normal();
  test_eq_undefined_or_null_string_max_min();
  test_eq_undefined_or_null_string_zero_max();
  test_eq_undefined_or_null_string_zero_zero();
  test_eq_undefined_or_null_string_zero_normal();
  test_eq_undefined_or_null_string_zero_min();
  test_eq_undefined_or_null_string_normal_max();
  test_eq_undefined_or_null_string_normal_zero();
  test_eq_undefined_or_null_string_normal_normal();
  test_eq_undefined_or_null_string_normal_min();
  test_eq_undefined_or_null_string_min_max();
  test_eq_undefined_or_null_string_min_zero();
  test_eq_undefined_or_null_string_min_normal();
  test_eq_undefined_or_null_string_min_min();
  test_eq_undefined_or_null_bool_max_max();
  test_eq_undefined_or_null_bool_max_zero();
  test_eq_undefined_or_null_bool_max_normal();
  test_eq_undefined_or_null_bool_max_min();
  test_eq_undefined_or_null_bool_zero_max();
  test_eq_undefined_or_null_bool_zero_zero();
  test_eq_undefined_or_null_bool_zero_normal();
  test_eq_undefined_or_null_bool_zero_min();
  test_eq_undefined_or_null_bool_normal_max();
  test_eq_undefined_or_null_bool_normal_zero();
  test_eq_undefined_or_null_bool_normal_normal();
  test_eq_undefined_or_null_bool_normal_min();
  test_eq_undefined_or_null_bool_min_max();
  test_eq_undefined_or_null_bool_min_zero();
  test_eq_undefined_or_null_bool_min_normal();
  test_eq_undefined_or_null_bool_min_min();
  test_eq_undefined_or_null_undefined_or_null_max_max();
  test_eq_undefined_or_null_undefined_or_null_max_zero();
  test_eq_undefined_or_null_undefined_or_null_max_normal();
  test_eq_undefined_or_null_undefined_or_null_max_min();
  test_eq_undefined_or_null_undefined_or_null_zero_max();
  test_eq_undefined_or_null_undefined_or_null_zero_zero();
  test_eq_undefined_or_null_undefined_or_null_zero_normal();
  test_eq_undefined_or_null_undefined_or_null_zero_min();
  test_eq_undefined_or_null_undefined_or_null_normal_max();
  test_eq_undefined_or_null_undefined_or_null_normal_zero();
  test_eq_undefined_or_null_undefined_or_null_normal_normal();
  test_eq_undefined_or_null_undefined_or_null_normal_min();
  test_eq_undefined_or_null_undefined_or_null_min_max();
  test_eq_undefined_or_null_undefined_or_null_min_zero();
  test_eq_undefined_or_null_undefined_or_null_min_normal();
  test_eq_undefined_or_null_undefined_or_null_min_min();

}
// ----------------------------------------------------------------