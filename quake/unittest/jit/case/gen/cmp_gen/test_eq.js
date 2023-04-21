// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


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


function test_eq_f64_f64_normal_max() {
  dump("testing: test_eq_f64_f64_normal_max");
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


function test_eq_f64_f64_normal_min() {
  dump("testing: test_eq_f64_f64_normal_min");
  let a0 = 13.0;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_nan() {
  dump("testing: test_eq_f64_f64_normal_nan");
  let a0 = 13.0;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_normal_pinf() {
  dump("testing: test_eq_f64_f64_normal_pinf");
  let a0 = 13.0;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_f64_normal_ninf() {
  dump("testing: test_eq_f64_f64_normal_ninf");
  let a0 = 13.0;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_f64_max_max() {
  dump("testing: test_eq_f64_f64_max_max");
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


function test_eq_f64_f64_max_min() {
  dump("testing: test_eq_f64_f64_max_min");
  let a0 = 1.79769313486e+308;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_max_nan() {
  dump("testing: test_eq_f64_f64_max_nan");
  let a0 = 1.79769313486e+308;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_max_pinf() {
  dump("testing: test_eq_f64_f64_max_pinf");
  let a0 = 1.79769313486e+308;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_f64_max_ninf() {
  dump("testing: test_eq_f64_f64_max_ninf");
  let a0 = 1.79769313486e+308;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_f64_min_max() {
  dump("testing: test_eq_f64_f64_min_max");
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_f64_min_min() {
  dump("testing: test_eq_f64_f64_min_min");
  let a0 = -1.79769313486e+308;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_nan() {
  dump("testing: test_eq_f64_f64_min_nan");
  let a0 = -1.79769313486e+308;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_min_pinf() {
  dump("testing: test_eq_f64_f64_min_pinf");
  let a0 = -1.79769313486e+308;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_f64_min_ninf() {
  dump("testing: test_eq_f64_f64_min_ninf");
  let a0 = -1.79769313486e+308;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_nan_normal() {
  dump("testing: test_eq_f64_f64_nan_normal");
  let a0 = NaN;
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


function test_eq_f64_f64_nan_max() {
  dump("testing: test_eq_f64_f64_nan_max");
  let a0 = NaN;
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


function test_eq_f64_f64_nan_min() {
  dump("testing: test_eq_f64_f64_nan_min");
  let a0 = NaN;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_nan_nan() {
  dump("testing: test_eq_f64_f64_nan_nan");
  let a0 = NaN;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_nan_pinf() {
  dump("testing: test_eq_f64_f64_nan_pinf");
  let a0 = NaN;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_nan_zero() {
  dump("testing: test_eq_f64_f64_nan_zero");
  let a0 = NaN;
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


function test_eq_f64_f64_nan_ninf() {
  dump("testing: test_eq_f64_f64_nan_ninf");
  let a0 = NaN;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_pinf_normal() {
  dump("testing: test_eq_f64_f64_pinf_normal");
  let a0 = -Infinity;
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


function test_eq_f64_f64_pinf_max() {
  dump("testing: test_eq_f64_f64_pinf_max");
  let a0 = -Infinity;
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


function test_eq_f64_f64_pinf_min() {
  dump("testing: test_eq_f64_f64_pinf_min");
  let a0 = -Infinity;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_pinf_nan() {
  dump("testing: test_eq_f64_f64_pinf_nan");
  let a0 = -Infinity;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_pinf_pinf() {
  dump("testing: test_eq_f64_f64_pinf_pinf");
  let a0 = -Infinity;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_pinf_zero() {
  dump("testing: test_eq_f64_f64_pinf_zero");
  let a0 = -Infinity;
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


function test_eq_f64_f64_pinf_ninf() {
  dump("testing: test_eq_f64_f64_pinf_ninf");
  let a0 = -Infinity;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_f64_zero_max() {
  dump("testing: test_eq_f64_f64_zero_max");
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


function test_eq_f64_f64_zero_min() {
  dump("testing: test_eq_f64_f64_zero_min");
  let a0 = 2.22507385851e-308;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_nan() {
  dump("testing: test_eq_f64_f64_zero_nan");
  let a0 = 2.22507385851e-308;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_zero_pinf() {
  dump("testing: test_eq_f64_f64_zero_pinf");
  let a0 = 2.22507385851e-308;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_f64_zero_ninf() {
  dump("testing: test_eq_f64_f64_zero_ninf");
  let a0 = 2.22507385851e-308;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_ninf_normal() {
  dump("testing: test_eq_f64_f64_ninf_normal");
  let a0 = Infinity;
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


function test_eq_f64_f64_ninf_max() {
  dump("testing: test_eq_f64_f64_ninf_max");
  let a0 = Infinity;
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


function test_eq_f64_f64_ninf_min() {
  dump("testing: test_eq_f64_f64_ninf_min");
  let a0 = Infinity;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_ninf_nan() {
  dump("testing: test_eq_f64_f64_ninf_nan");
  let a0 = Infinity;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_ninf_pinf() {
  dump("testing: test_eq_f64_f64_ninf_pinf");
  let a0 = Infinity;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_f64_ninf_zero() {
  dump("testing: test_eq_f64_f64_ninf_zero");
  let a0 = Infinity;
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


function test_eq_f64_f64_ninf_ninf() {
  dump("testing: test_eq_f64_f64_ninf_ninf");
  let a0 = Infinity;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_i32_max_max() {
  dump("testing: test_eq_f64_i32_max_max");
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


function test_eq_f64_i32_max_zero() {
  dump("testing: test_eq_f64_i32_max_zero");
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


function test_eq_f64_i32_max_normal() {
  dump("testing: test_eq_f64_i32_max_normal");
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


function test_eq_f64_i32_max_min() {
  dump("testing: test_eq_f64_i32_max_min");
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


function test_eq_f64_i32_min_max() {
  dump("testing: test_eq_f64_i32_min_max");
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_i32_nan_max() {
  dump("testing: test_eq_f64_i32_nan_max");
  let a0 = NaN;
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


function test_eq_f64_i32_nan_zero() {
  dump("testing: test_eq_f64_i32_nan_zero");
  let a0 = NaN;
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


function test_eq_f64_i32_nan_normal() {
  dump("testing: test_eq_f64_i32_nan_normal");
  let a0 = NaN;
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


function test_eq_f64_i32_nan_min() {
  dump("testing: test_eq_f64_i32_nan_min");
  let a0 = NaN;
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


function test_eq_f64_i32_pinf_max() {
  dump("testing: test_eq_f64_i32_pinf_max");
  let a0 = -Infinity;
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


function test_eq_f64_i32_pinf_zero() {
  dump("testing: test_eq_f64_i32_pinf_zero");
  let a0 = -Infinity;
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


function test_eq_f64_i32_pinf_normal() {
  dump("testing: test_eq_f64_i32_pinf_normal");
  let a0 = -Infinity;
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


function test_eq_f64_i32_pinf_min() {
  dump("testing: test_eq_f64_i32_pinf_min");
  let a0 = -Infinity;
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


function test_eq_f64_i32_zero_zero() {
  dump("testing: test_eq_f64_i32_zero_zero");
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


function test_eq_f64_i32_zero_normal() {
  dump("testing: test_eq_f64_i32_zero_normal");
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


function test_eq_f64_i32_zero_min() {
  dump("testing: test_eq_f64_i32_zero_min");
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


function test_eq_f64_i32_ninf_max() {
  dump("testing: test_eq_f64_i32_ninf_max");
  let a0 = Infinity;
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


function test_eq_f64_i32_ninf_zero() {
  dump("testing: test_eq_f64_i32_ninf_zero");
  let a0 = Infinity;
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


function test_eq_f64_i32_ninf_normal() {
  dump("testing: test_eq_f64_i32_ninf_normal");
  let a0 = Infinity;
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


function test_eq_f64_i32_ninf_min() {
  dump("testing: test_eq_f64_i32_ninf_min");
  let a0 = Infinity;
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


function test_eq_f64_string_max_max() {
  dump("testing: test_eq_f64_string_max_max");
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


function test_eq_f64_string_max_zero() {
  dump("testing: test_eq_f64_string_max_zero");
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


function test_eq_f64_string_max_normal() {
  dump("testing: test_eq_f64_string_max_normal");
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


function test_eq_f64_string_max_min() {
  dump("testing: test_eq_f64_string_max_min");
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


function test_eq_f64_string_min_max() {
  dump("testing: test_eq_f64_string_min_max");
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_string_nan_max() {
  dump("testing: test_eq_f64_string_nan_max");
  let a0 = NaN;
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


function test_eq_f64_string_nan_zero() {
  dump("testing: test_eq_f64_string_nan_zero");
  let a0 = NaN;
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


function test_eq_f64_string_nan_normal() {
  dump("testing: test_eq_f64_string_nan_normal");
  let a0 = NaN;
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


function test_eq_f64_string_nan_min() {
  dump("testing: test_eq_f64_string_nan_min");
  let a0 = NaN;
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


function test_eq_f64_string_pinf_max() {
  dump("testing: test_eq_f64_string_pinf_max");
  let a0 = -Infinity;
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


function test_eq_f64_string_pinf_zero() {
  dump("testing: test_eq_f64_string_pinf_zero");
  let a0 = -Infinity;
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


function test_eq_f64_string_pinf_normal() {
  dump("testing: test_eq_f64_string_pinf_normal");
  let a0 = -Infinity;
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


function test_eq_f64_string_pinf_min() {
  dump("testing: test_eq_f64_string_pinf_min");
  let a0 = -Infinity;
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


function test_eq_f64_string_zero_zero() {
  dump("testing: test_eq_f64_string_zero_zero");
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


function test_eq_f64_string_zero_normal() {
  dump("testing: test_eq_f64_string_zero_normal");
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


function test_eq_f64_string_zero_min() {
  dump("testing: test_eq_f64_string_zero_min");
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


function test_eq_f64_string_ninf_max() {
  dump("testing: test_eq_f64_string_ninf_max");
  let a0 = Infinity;
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


function test_eq_f64_string_ninf_zero() {
  dump("testing: test_eq_f64_string_ninf_zero");
  let a0 = Infinity;
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


function test_eq_f64_string_ninf_normal() {
  dump("testing: test_eq_f64_string_ninf_normal");
  let a0 = Infinity;
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


function test_eq_f64_string_ninf_min() {
  dump("testing: test_eq_f64_string_ninf_min");
  let a0 = Infinity;
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


function test_eq_f64_bool_max_max() {
  dump("testing: test_eq_f64_bool_max_max");
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


function test_eq_f64_bool_max_zero() {
  dump("testing: test_eq_f64_bool_max_zero");
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


function test_eq_f64_bool_max_normal() {
  dump("testing: test_eq_f64_bool_max_normal");
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


function test_eq_f64_bool_max_min() {
  dump("testing: test_eq_f64_bool_max_min");
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


function test_eq_f64_bool_min_max() {
  dump("testing: test_eq_f64_bool_min_max");
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_bool_nan_max() {
  dump("testing: test_eq_f64_bool_nan_max");
  let a0 = NaN;
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


function test_eq_f64_bool_nan_zero() {
  dump("testing: test_eq_f64_bool_nan_zero");
  let a0 = NaN;
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


function test_eq_f64_bool_nan_normal() {
  dump("testing: test_eq_f64_bool_nan_normal");
  let a0 = NaN;
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


function test_eq_f64_bool_nan_min() {
  dump("testing: test_eq_f64_bool_nan_min");
  let a0 = NaN;
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


function test_eq_f64_bool_pinf_max() {
  dump("testing: test_eq_f64_bool_pinf_max");
  let a0 = -Infinity;
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


function test_eq_f64_bool_pinf_zero() {
  dump("testing: test_eq_f64_bool_pinf_zero");
  let a0 = -Infinity;
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


function test_eq_f64_bool_pinf_normal() {
  dump("testing: test_eq_f64_bool_pinf_normal");
  let a0 = -Infinity;
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


function test_eq_f64_bool_pinf_min() {
  dump("testing: test_eq_f64_bool_pinf_min");
  let a0 = -Infinity;
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


function test_eq_f64_bool_zero_zero() {
  dump("testing: test_eq_f64_bool_zero_zero");
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


function test_eq_f64_bool_zero_normal() {
  dump("testing: test_eq_f64_bool_zero_normal");
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


function test_eq_f64_bool_zero_min() {
  dump("testing: test_eq_f64_bool_zero_min");
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


function test_eq_f64_bool_ninf_max() {
  dump("testing: test_eq_f64_bool_ninf_max");
  let a0 = Infinity;
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


function test_eq_f64_bool_ninf_zero() {
  dump("testing: test_eq_f64_bool_ninf_zero");
  let a0 = Infinity;
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


function test_eq_f64_bool_ninf_normal() {
  dump("testing: test_eq_f64_bool_ninf_normal");
  let a0 = Infinity;
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


function test_eq_f64_bool_ninf_min() {
  dump("testing: test_eq_f64_bool_ninf_min");
  let a0 = Infinity;
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


function test_eq_f64_stri32_normal_max() {
  dump("testing: test_eq_f64_stri32_normal_max");
  let a0 = 13.0;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_normal_zero() {
  dump("testing: test_eq_f64_stri32_normal_zero");
  let a0 = 13.0;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_normal_normal() {
  dump("testing: test_eq_f64_stri32_normal_normal");
  let a0 = 13.0;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_normal_min() {
  dump("testing: test_eq_f64_stri32_normal_min");
  let a0 = 13.0;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_max_max() {
  dump("testing: test_eq_f64_stri32_max_max");
  let a0 = 1.79769313486e+308;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_max_zero() {
  dump("testing: test_eq_f64_stri32_max_zero");
  let a0 = 1.79769313486e+308;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_max_normal() {
  dump("testing: test_eq_f64_stri32_max_normal");
  let a0 = 1.79769313486e+308;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_max_min() {
  dump("testing: test_eq_f64_stri32_max_min");
  let a0 = 1.79769313486e+308;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_min_max() {
  dump("testing: test_eq_f64_stri32_min_max");
  let a0 = -1.79769313486e+308;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_min_zero() {
  dump("testing: test_eq_f64_stri32_min_zero");
  let a0 = -1.79769313486e+308;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_min_normal() {
  dump("testing: test_eq_f64_stri32_min_normal");
  let a0 = -1.79769313486e+308;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_min_min() {
  dump("testing: test_eq_f64_stri32_min_min");
  let a0 = -1.79769313486e+308;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_nan_max() {
  dump("testing: test_eq_f64_stri32_nan_max");
  let a0 = NaN;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_nan_zero() {
  dump("testing: test_eq_f64_stri32_nan_zero");
  let a0 = NaN;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_nan_normal() {
  dump("testing: test_eq_f64_stri32_nan_normal");
  let a0 = NaN;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_nan_min() {
  dump("testing: test_eq_f64_stri32_nan_min");
  let a0 = NaN;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_pinf_max() {
  dump("testing: test_eq_f64_stri32_pinf_max");
  let a0 = -Infinity;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_pinf_zero() {
  dump("testing: test_eq_f64_stri32_pinf_zero");
  let a0 = -Infinity;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_pinf_normal() {
  dump("testing: test_eq_f64_stri32_pinf_normal");
  let a0 = -Infinity;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_pinf_min() {
  dump("testing: test_eq_f64_stri32_pinf_min");
  let a0 = -Infinity;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_zero_max() {
  dump("testing: test_eq_f64_stri32_zero_max");
  let a0 = 2.22507385851e-308;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_zero_zero() {
  dump("testing: test_eq_f64_stri32_zero_zero");
  let a0 = 2.22507385851e-308;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_zero_normal() {
  dump("testing: test_eq_f64_stri32_zero_normal");
  let a0 = 2.22507385851e-308;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_zero_min() {
  dump("testing: test_eq_f64_stri32_zero_min");
  let a0 = 2.22507385851e-308;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_ninf_max() {
  dump("testing: test_eq_f64_stri32_ninf_max");
  let a0 = Infinity;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_ninf_zero() {
  dump("testing: test_eq_f64_stri32_ninf_zero");
  let a0 = Infinity;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_ninf_normal() {
  dump("testing: test_eq_f64_stri32_ninf_normal");
  let a0 = Infinity;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_stri32_ninf_min() {
  dump("testing: test_eq_f64_stri32_ninf_min");
  let a0 = Infinity;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_normal_max() {
  dump("testing: test_eq_f64_strf64_normal_max");
  let a0 = 13.0;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_normal_zero() {
  dump("testing: test_eq_f64_strf64_normal_zero");
  let a0 = 13.0;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_normal_normal() {
  dump("testing: test_eq_f64_strf64_normal_normal");
  let a0 = 13.0;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_normal_min() {
  dump("testing: test_eq_f64_strf64_normal_min");
  let a0 = 13.0;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_max_max() {
  dump("testing: test_eq_f64_strf64_max_max");
  let a0 = 1.79769313486e+308;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_max_zero() {
  dump("testing: test_eq_f64_strf64_max_zero");
  let a0 = 1.79769313486e+308;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_max_normal() {
  dump("testing: test_eq_f64_strf64_max_normal");
  let a0 = 1.79769313486e+308;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_max_min() {
  dump("testing: test_eq_f64_strf64_max_min");
  let a0 = 1.79769313486e+308;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_min_max() {
  dump("testing: test_eq_f64_strf64_min_max");
  let a0 = -1.79769313486e+308;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_min_zero() {
  dump("testing: test_eq_f64_strf64_min_zero");
  let a0 = -1.79769313486e+308;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_min_normal() {
  dump("testing: test_eq_f64_strf64_min_normal");
  let a0 = -1.79769313486e+308;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_min_min() {
  dump("testing: test_eq_f64_strf64_min_min");
  let a0 = -1.79769313486e+308;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_nan_max() {
  dump("testing: test_eq_f64_strf64_nan_max");
  let a0 = NaN;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_nan_zero() {
  dump("testing: test_eq_f64_strf64_nan_zero");
  let a0 = NaN;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_nan_normal() {
  dump("testing: test_eq_f64_strf64_nan_normal");
  let a0 = NaN;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_nan_min() {
  dump("testing: test_eq_f64_strf64_nan_min");
  let a0 = NaN;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_pinf_max() {
  dump("testing: test_eq_f64_strf64_pinf_max");
  let a0 = -Infinity;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_pinf_zero() {
  dump("testing: test_eq_f64_strf64_pinf_zero");
  let a0 = -Infinity;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_pinf_normal() {
  dump("testing: test_eq_f64_strf64_pinf_normal");
  let a0 = -Infinity;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_pinf_min() {
  dump("testing: test_eq_f64_strf64_pinf_min");
  let a0 = -Infinity;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_zero_max() {
  dump("testing: test_eq_f64_strf64_zero_max");
  let a0 = 2.22507385851e-308;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_zero_zero() {
  dump("testing: test_eq_f64_strf64_zero_zero");
  let a0 = 2.22507385851e-308;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_zero_normal() {
  dump("testing: test_eq_f64_strf64_zero_normal");
  let a0 = 2.22507385851e-308;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_zero_min() {
  dump("testing: test_eq_f64_strf64_zero_min");
  let a0 = 2.22507385851e-308;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_ninf_max() {
  dump("testing: test_eq_f64_strf64_ninf_max");
  let a0 = Infinity;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_ninf_zero() {
  dump("testing: test_eq_f64_strf64_ninf_zero");
  let a0 = Infinity;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_ninf_normal() {
  dump("testing: test_eq_f64_strf64_ninf_normal");
  let a0 = Infinity;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_f64_strf64_ninf_min() {
  dump("testing: test_eq_f64_strf64_ninf_min");
  let a0 = Infinity;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_f64_undefined_or_null_max_max() {
  dump("testing: test_eq_f64_undefined_or_null_max_max");
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


function test_eq_f64_undefined_or_null_max_zero() {
  dump("testing: test_eq_f64_undefined_or_null_max_zero");
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


function test_eq_f64_undefined_or_null_max_normal() {
  dump("testing: test_eq_f64_undefined_or_null_max_normal");
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


function test_eq_f64_undefined_or_null_max_min() {
  dump("testing: test_eq_f64_undefined_or_null_max_min");
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


function test_eq_f64_undefined_or_null_min_max() {
  dump("testing: test_eq_f64_undefined_or_null_min_max");
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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
  let a0 = -1.79769313486e+308;
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


function test_eq_f64_undefined_or_null_nan_max() {
  dump("testing: test_eq_f64_undefined_or_null_nan_max");
  let a0 = NaN;
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


function test_eq_f64_undefined_or_null_nan_zero() {
  dump("testing: test_eq_f64_undefined_or_null_nan_zero");
  let a0 = NaN;
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


function test_eq_f64_undefined_or_null_nan_normal() {
  dump("testing: test_eq_f64_undefined_or_null_nan_normal");
  let a0 = NaN;
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


function test_eq_f64_undefined_or_null_nan_min() {
  dump("testing: test_eq_f64_undefined_or_null_nan_min");
  let a0 = NaN;
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


function test_eq_f64_undefined_or_null_pinf_max() {
  dump("testing: test_eq_f64_undefined_or_null_pinf_max");
  let a0 = -Infinity;
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


function test_eq_f64_undefined_or_null_pinf_zero() {
  dump("testing: test_eq_f64_undefined_or_null_pinf_zero");
  let a0 = -Infinity;
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


function test_eq_f64_undefined_or_null_pinf_normal() {
  dump("testing: test_eq_f64_undefined_or_null_pinf_normal");
  let a0 = -Infinity;
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


function test_eq_f64_undefined_or_null_pinf_min() {
  dump("testing: test_eq_f64_undefined_or_null_pinf_min");
  let a0 = -Infinity;
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


function test_eq_f64_undefined_or_null_zero_zero() {
  dump("testing: test_eq_f64_undefined_or_null_zero_zero");
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


function test_eq_f64_undefined_or_null_zero_normal() {
  dump("testing: test_eq_f64_undefined_or_null_zero_normal");
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


function test_eq_f64_undefined_or_null_zero_min() {
  dump("testing: test_eq_f64_undefined_or_null_zero_min");
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


function test_eq_f64_undefined_or_null_ninf_max() {
  dump("testing: test_eq_f64_undefined_or_null_ninf_max");
  let a0 = Infinity;
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


function test_eq_f64_undefined_or_null_ninf_zero() {
  dump("testing: test_eq_f64_undefined_or_null_ninf_zero");
  let a0 = Infinity;
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


function test_eq_f64_undefined_or_null_ninf_normal() {
  dump("testing: test_eq_f64_undefined_or_null_ninf_normal");
  let a0 = Infinity;
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


function test_eq_f64_undefined_or_null_ninf_min() {
  dump("testing: test_eq_f64_undefined_or_null_ninf_min");
  let a0 = Infinity;
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


function test_eq_i32_f64_max_max() {
  dump("testing: test_eq_i32_f64_max_max");
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


function test_eq_i32_f64_max_min() {
  dump("testing: test_eq_i32_f64_max_min");
  let a0 = 2147483647;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_nan() {
  dump("testing: test_eq_i32_f64_max_nan");
  let a0 = 2147483647;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_max_pinf() {
  dump("testing: test_eq_i32_f64_max_pinf");
  let a0 = 2147483647;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_max_ninf() {
  dump("testing: test_eq_i32_f64_max_ninf");
  let a0 = 2147483647;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_zero_max() {
  dump("testing: test_eq_i32_f64_zero_max");
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


function test_eq_i32_f64_zero_min() {
  dump("testing: test_eq_i32_f64_zero_min");
  let a0 = 0;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_nan() {
  dump("testing: test_eq_i32_f64_zero_nan");
  let a0 = 0;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_zero_pinf() {
  dump("testing: test_eq_i32_f64_zero_pinf");
  let a0 = 0;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_zero_ninf() {
  dump("testing: test_eq_i32_f64_zero_ninf");
  let a0 = 0;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_normal_max() {
  dump("testing: test_eq_i32_f64_normal_max");
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


function test_eq_i32_f64_normal_min() {
  dump("testing: test_eq_i32_f64_normal_min");
  let a0 = 14;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_nan() {
  dump("testing: test_eq_i32_f64_normal_nan");
  let a0 = 14;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_normal_pinf() {
  dump("testing: test_eq_i32_f64_normal_pinf");
  let a0 = 14;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_normal_ninf() {
  dump("testing: test_eq_i32_f64_normal_ninf");
  let a0 = 14;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_min_max() {
  dump("testing: test_eq_i32_f64_min_max");
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


function test_eq_i32_f64_min_min() {
  dump("testing: test_eq_i32_f64_min_min");
  let a0 = -2147483648;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_nan() {
  dump("testing: test_eq_i32_f64_min_nan");
  let a0 = -2147483648;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_f64_min_pinf() {
  dump("testing: test_eq_i32_f64_min_pinf");
  let a0 = -2147483648;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_f64_min_ninf() {
  dump("testing: test_eq_i32_f64_min_ninf");
  let a0 = -2147483648;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_i32_stri32_max_max() {
  dump("testing: test_eq_i32_stri32_max_max");
  let a0 = 2147483647;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_max_zero() {
  dump("testing: test_eq_i32_stri32_max_zero");
  let a0 = 2147483647;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_max_normal() {
  dump("testing: test_eq_i32_stri32_max_normal");
  let a0 = 2147483647;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_max_min() {
  dump("testing: test_eq_i32_stri32_max_min");
  let a0 = 2147483647;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_zero_max() {
  dump("testing: test_eq_i32_stri32_zero_max");
  let a0 = 0;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_zero_zero() {
  dump("testing: test_eq_i32_stri32_zero_zero");
  let a0 = 0;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_zero_normal() {
  dump("testing: test_eq_i32_stri32_zero_normal");
  let a0 = 0;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_zero_min() {
  dump("testing: test_eq_i32_stri32_zero_min");
  let a0 = 0;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_normal_max() {
  dump("testing: test_eq_i32_stri32_normal_max");
  let a0 = 14;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_normal_zero() {
  dump("testing: test_eq_i32_stri32_normal_zero");
  let a0 = 14;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_normal_normal() {
  dump("testing: test_eq_i32_stri32_normal_normal");
  let a0 = 14;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_normal_min() {
  dump("testing: test_eq_i32_stri32_normal_min");
  let a0 = 14;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_min_max() {
  dump("testing: test_eq_i32_stri32_min_max");
  let a0 = -2147483648;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_min_zero() {
  dump("testing: test_eq_i32_stri32_min_zero");
  let a0 = -2147483648;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_min_normal() {
  dump("testing: test_eq_i32_stri32_min_normal");
  let a0 = -2147483648;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_stri32_min_min() {
  dump("testing: test_eq_i32_stri32_min_min");
  let a0 = -2147483648;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_max_max() {
  dump("testing: test_eq_i32_strf64_max_max");
  let a0 = 2147483647;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_max_zero() {
  dump("testing: test_eq_i32_strf64_max_zero");
  let a0 = 2147483647;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_max_normal() {
  dump("testing: test_eq_i32_strf64_max_normal");
  let a0 = 2147483647;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_max_min() {
  dump("testing: test_eq_i32_strf64_max_min");
  let a0 = 2147483647;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_zero_max() {
  dump("testing: test_eq_i32_strf64_zero_max");
  let a0 = 0;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_zero_zero() {
  dump("testing: test_eq_i32_strf64_zero_zero");
  let a0 = 0;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_zero_normal() {
  dump("testing: test_eq_i32_strf64_zero_normal");
  let a0 = 0;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_zero_min() {
  dump("testing: test_eq_i32_strf64_zero_min");
  let a0 = 0;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_normal_max() {
  dump("testing: test_eq_i32_strf64_normal_max");
  let a0 = 14;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_normal_zero() {
  dump("testing: test_eq_i32_strf64_normal_zero");
  let a0 = 14;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_normal_normal() {
  dump("testing: test_eq_i32_strf64_normal_normal");
  let a0 = 14;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_normal_min() {
  dump("testing: test_eq_i32_strf64_normal_min");
  let a0 = 14;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_min_max() {
  dump("testing: test_eq_i32_strf64_min_max");
  let a0 = -2147483648;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_min_zero() {
  dump("testing: test_eq_i32_strf64_min_zero");
  let a0 = -2147483648;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_min_normal() {
  dump("testing: test_eq_i32_strf64_min_normal");
  let a0 = -2147483648;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_i32_strf64_min_min() {
  dump("testing: test_eq_i32_strf64_min_min");
  let a0 = -2147483648;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_max_max() {
  dump("testing: test_eq_string_f64_max_max");
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


function test_eq_string_f64_max_min() {
  dump("testing: test_eq_string_f64_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_nan() {
  dump("testing: test_eq_string_f64_max_nan");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_max_pinf() {
  dump("testing: test_eq_string_f64_max_pinf");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_max_ninf() {
  dump("testing: test_eq_string_f64_max_ninf");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_zero_max() {
  dump("testing: test_eq_string_f64_zero_max");
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


function test_eq_string_f64_zero_min() {
  dump("testing: test_eq_string_f64_zero_min");
  let a0 = '';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_nan() {
  dump("testing: test_eq_string_f64_zero_nan");
  let a0 = '';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_zero_pinf() {
  dump("testing: test_eq_string_f64_zero_pinf");
  let a0 = '';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_zero_ninf() {
  dump("testing: test_eq_string_f64_zero_ninf");
  let a0 = '';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_normal_max() {
  dump("testing: test_eq_string_f64_normal_max");
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


function test_eq_string_f64_normal_min() {
  dump("testing: test_eq_string_f64_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_nan() {
  dump("testing: test_eq_string_f64_normal_nan");
  let a0 = 'aaaabbccc';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_normal_pinf() {
  dump("testing: test_eq_string_f64_normal_pinf");
  let a0 = 'aaaabbccc';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_normal_ninf() {
  dump("testing: test_eq_string_f64_normal_ninf");
  let a0 = 'aaaabbccc';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_min_max() {
  dump("testing: test_eq_string_f64_min_max");
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


function test_eq_string_f64_min_min() {
  dump("testing: test_eq_string_f64_min_min");
  let a0 = '';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_nan() {
  dump("testing: test_eq_string_f64_min_nan");
  let a0 = '';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_f64_min_pinf() {
  dump("testing: test_eq_string_f64_min_pinf");
  let a0 = '';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_f64_min_ninf() {
  dump("testing: test_eq_string_f64_min_ninf");
  let a0 = '';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_string_stri32_max_max() {
  dump("testing: test_eq_string_stri32_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_max_zero() {
  dump("testing: test_eq_string_stri32_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_max_normal() {
  dump("testing: test_eq_string_stri32_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_max_min() {
  dump("testing: test_eq_string_stri32_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_zero_max() {
  dump("testing: test_eq_string_stri32_zero_max");
  let a0 = '';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_zero_zero() {
  dump("testing: test_eq_string_stri32_zero_zero");
  let a0 = '';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_zero_normal() {
  dump("testing: test_eq_string_stri32_zero_normal");
  let a0 = '';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_zero_min() {
  dump("testing: test_eq_string_stri32_zero_min");
  let a0 = '';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_normal_max() {
  dump("testing: test_eq_string_stri32_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_normal_zero() {
  dump("testing: test_eq_string_stri32_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_normal_normal() {
  dump("testing: test_eq_string_stri32_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_normal_min() {
  dump("testing: test_eq_string_stri32_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_min_max() {
  dump("testing: test_eq_string_stri32_min_max");
  let a0 = '';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_min_zero() {
  dump("testing: test_eq_string_stri32_min_zero");
  let a0 = '';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_min_normal() {
  dump("testing: test_eq_string_stri32_min_normal");
  let a0 = '';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_stri32_min_min() {
  dump("testing: test_eq_string_stri32_min_min");
  let a0 = '';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_max_max() {
  dump("testing: test_eq_string_strf64_max_max");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_max_zero() {
  dump("testing: test_eq_string_strf64_max_zero");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_max_normal() {
  dump("testing: test_eq_string_strf64_max_normal");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_max_min() {
  dump("testing: test_eq_string_strf64_max_min");
  let a0 = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_zero_max() {
  dump("testing: test_eq_string_strf64_zero_max");
  let a0 = '';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_zero_zero() {
  dump("testing: test_eq_string_strf64_zero_zero");
  let a0 = '';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_zero_normal() {
  dump("testing: test_eq_string_strf64_zero_normal");
  let a0 = '';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_zero_min() {
  dump("testing: test_eq_string_strf64_zero_min");
  let a0 = '';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_normal_max() {
  dump("testing: test_eq_string_strf64_normal_max");
  let a0 = 'aaaabbccc';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_normal_zero() {
  dump("testing: test_eq_string_strf64_normal_zero");
  let a0 = 'aaaabbccc';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_normal_normal() {
  dump("testing: test_eq_string_strf64_normal_normal");
  let a0 = 'aaaabbccc';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_normal_min() {
  dump("testing: test_eq_string_strf64_normal_min");
  let a0 = 'aaaabbccc';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_min_max() {
  dump("testing: test_eq_string_strf64_min_max");
  let a0 = '';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_min_zero() {
  dump("testing: test_eq_string_strf64_min_zero");
  let a0 = '';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_min_normal() {
  dump("testing: test_eq_string_strf64_min_normal");
  let a0 = '';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_string_strf64_min_min() {
  dump("testing: test_eq_string_strf64_min_min");
  let a0 = '';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_max_max() {
  dump("testing: test_eq_bool_f64_max_max");
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


function test_eq_bool_f64_max_min() {
  dump("testing: test_eq_bool_f64_max_min");
  let a0 = true;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_nan() {
  dump("testing: test_eq_bool_f64_max_nan");
  let a0 = true;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_max_pinf() {
  dump("testing: test_eq_bool_f64_max_pinf");
  let a0 = true;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_max_ninf() {
  dump("testing: test_eq_bool_f64_max_ninf");
  let a0 = true;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_zero_max() {
  dump("testing: test_eq_bool_f64_zero_max");
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


function test_eq_bool_f64_zero_min() {
  dump("testing: test_eq_bool_f64_zero_min");
  let a0 = false;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_nan() {
  dump("testing: test_eq_bool_f64_zero_nan");
  let a0 = false;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_zero_pinf() {
  dump("testing: test_eq_bool_f64_zero_pinf");
  let a0 = false;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_zero_ninf() {
  dump("testing: test_eq_bool_f64_zero_ninf");
  let a0 = false;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_normal_max() {
  dump("testing: test_eq_bool_f64_normal_max");
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


function test_eq_bool_f64_normal_min() {
  dump("testing: test_eq_bool_f64_normal_min");
  let a0 = true;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_nan() {
  dump("testing: test_eq_bool_f64_normal_nan");
  let a0 = true;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_normal_pinf() {
  dump("testing: test_eq_bool_f64_normal_pinf");
  let a0 = true;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_normal_ninf() {
  dump("testing: test_eq_bool_f64_normal_ninf");
  let a0 = true;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_min_max() {
  dump("testing: test_eq_bool_f64_min_max");
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


function test_eq_bool_f64_min_min() {
  dump("testing: test_eq_bool_f64_min_min");
  let a0 = false;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_nan() {
  dump("testing: test_eq_bool_f64_min_nan");
  let a0 = false;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_f64_min_pinf() {
  dump("testing: test_eq_bool_f64_min_pinf");
  let a0 = false;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_f64_min_ninf() {
  dump("testing: test_eq_bool_f64_min_ninf");
  let a0 = false;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_bool_stri32_max_max() {
  dump("testing: test_eq_bool_stri32_max_max");
  let a0 = true;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_max_zero() {
  dump("testing: test_eq_bool_stri32_max_zero");
  let a0 = true;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_max_normal() {
  dump("testing: test_eq_bool_stri32_max_normal");
  let a0 = true;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_max_min() {
  dump("testing: test_eq_bool_stri32_max_min");
  let a0 = true;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_zero_max() {
  dump("testing: test_eq_bool_stri32_zero_max");
  let a0 = false;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_zero_zero() {
  dump("testing: test_eq_bool_stri32_zero_zero");
  let a0 = false;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_zero_normal() {
  dump("testing: test_eq_bool_stri32_zero_normal");
  let a0 = false;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_zero_min() {
  dump("testing: test_eq_bool_stri32_zero_min");
  let a0 = false;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_normal_max() {
  dump("testing: test_eq_bool_stri32_normal_max");
  let a0 = true;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_normal_zero() {
  dump("testing: test_eq_bool_stri32_normal_zero");
  let a0 = true;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_normal_normal() {
  dump("testing: test_eq_bool_stri32_normal_normal");
  let a0 = true;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_normal_min() {
  dump("testing: test_eq_bool_stri32_normal_min");
  let a0 = true;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_min_max() {
  dump("testing: test_eq_bool_stri32_min_max");
  let a0 = false;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_min_zero() {
  dump("testing: test_eq_bool_stri32_min_zero");
  let a0 = false;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_min_normal() {
  dump("testing: test_eq_bool_stri32_min_normal");
  let a0 = false;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_stri32_min_min() {
  dump("testing: test_eq_bool_stri32_min_min");
  let a0 = false;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_max_max() {
  dump("testing: test_eq_bool_strf64_max_max");
  let a0 = true;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_max_zero() {
  dump("testing: test_eq_bool_strf64_max_zero");
  let a0 = true;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_max_normal() {
  dump("testing: test_eq_bool_strf64_max_normal");
  let a0 = true;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_max_min() {
  dump("testing: test_eq_bool_strf64_max_min");
  let a0 = true;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_zero_max() {
  dump("testing: test_eq_bool_strf64_zero_max");
  let a0 = false;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_zero_zero() {
  dump("testing: test_eq_bool_strf64_zero_zero");
  let a0 = false;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_zero_normal() {
  dump("testing: test_eq_bool_strf64_zero_normal");
  let a0 = false;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_zero_min() {
  dump("testing: test_eq_bool_strf64_zero_min");
  let a0 = false;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_normal_max() {
  dump("testing: test_eq_bool_strf64_normal_max");
  let a0 = true;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_normal_zero() {
  dump("testing: test_eq_bool_strf64_normal_zero");
  let a0 = true;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_normal_normal() {
  dump("testing: test_eq_bool_strf64_normal_normal");
  let a0 = true;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_normal_min() {
  dump("testing: test_eq_bool_strf64_normal_min");
  let a0 = true;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_min_max() {
  dump("testing: test_eq_bool_strf64_min_max");
  let a0 = false;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_min_zero() {
  dump("testing: test_eq_bool_strf64_min_zero");
  let a0 = false;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_min_normal() {
  dump("testing: test_eq_bool_strf64_min_normal");
  let a0 = false;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_bool_strf64_min_min() {
  dump("testing: test_eq_bool_strf64_min_min");
  let a0 = false;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_stri32_f64_max_normal() {
  dump("testing: test_eq_stri32_f64_max_normal");
  let a0 = '2147483647';
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


function test_eq_stri32_f64_max_max() {
  dump("testing: test_eq_stri32_f64_max_max");
  let a0 = '2147483647';
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


function test_eq_stri32_f64_max_min() {
  dump("testing: test_eq_stri32_f64_max_min");
  let a0 = '2147483647';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_max_nan() {
  dump("testing: test_eq_stri32_f64_max_nan");
  let a0 = '2147483647';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_max_pinf() {
  dump("testing: test_eq_stri32_f64_max_pinf");
  let a0 = '2147483647';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_max_zero() {
  dump("testing: test_eq_stri32_f64_max_zero");
  let a0 = '2147483647';
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


function test_eq_stri32_f64_max_ninf() {
  dump("testing: test_eq_stri32_f64_max_ninf");
  let a0 = '2147483647';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_zero_normal() {
  dump("testing: test_eq_stri32_f64_zero_normal");
  let a0 = '1';
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


function test_eq_stri32_f64_zero_max() {
  dump("testing: test_eq_stri32_f64_zero_max");
  let a0 = '1';
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


function test_eq_stri32_f64_zero_min() {
  dump("testing: test_eq_stri32_f64_zero_min");
  let a0 = '1';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_zero_nan() {
  dump("testing: test_eq_stri32_f64_zero_nan");
  let a0 = '1';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_zero_pinf() {
  dump("testing: test_eq_stri32_f64_zero_pinf");
  let a0 = '1';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_zero_zero() {
  dump("testing: test_eq_stri32_f64_zero_zero");
  let a0 = '1';
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


function test_eq_stri32_f64_zero_ninf() {
  dump("testing: test_eq_stri32_f64_zero_ninf");
  let a0 = '1';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_normal_normal() {
  dump("testing: test_eq_stri32_f64_normal_normal");
  let a0 = '1000';
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


function test_eq_stri32_f64_normal_max() {
  dump("testing: test_eq_stri32_f64_normal_max");
  let a0 = '1000';
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


function test_eq_stri32_f64_normal_min() {
  dump("testing: test_eq_stri32_f64_normal_min");
  let a0 = '1000';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_normal_nan() {
  dump("testing: test_eq_stri32_f64_normal_nan");
  let a0 = '1000';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_normal_pinf() {
  dump("testing: test_eq_stri32_f64_normal_pinf");
  let a0 = '1000';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_normal_zero() {
  dump("testing: test_eq_stri32_f64_normal_zero");
  let a0 = '1000';
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


function test_eq_stri32_f64_normal_ninf() {
  dump("testing: test_eq_stri32_f64_normal_ninf");
  let a0 = '1000';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_min_normal() {
  dump("testing: test_eq_stri32_f64_min_normal");
  let a0 = '-2147483648';
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


function test_eq_stri32_f64_min_max() {
  dump("testing: test_eq_stri32_f64_min_max");
  let a0 = '-2147483648';
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


function test_eq_stri32_f64_min_min() {
  dump("testing: test_eq_stri32_f64_min_min");
  let a0 = '-2147483648';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_min_nan() {
  dump("testing: test_eq_stri32_f64_min_nan");
  let a0 = '-2147483648';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_min_pinf() {
  dump("testing: test_eq_stri32_f64_min_pinf");
  let a0 = '-2147483648';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_f64_min_zero() {
  dump("testing: test_eq_stri32_f64_min_zero");
  let a0 = '-2147483648';
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


function test_eq_stri32_f64_min_ninf() {
  dump("testing: test_eq_stri32_f64_min_ninf");
  let a0 = '-2147483648';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_i32_max_max() {
  dump("testing: test_eq_stri32_i32_max_max");
  let a0 = '2147483647';
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


function test_eq_stri32_i32_max_zero() {
  dump("testing: test_eq_stri32_i32_max_zero");
  let a0 = '2147483647';
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


function test_eq_stri32_i32_max_normal() {
  dump("testing: test_eq_stri32_i32_max_normal");
  let a0 = '2147483647';
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


function test_eq_stri32_i32_max_min() {
  dump("testing: test_eq_stri32_i32_max_min");
  let a0 = '2147483647';
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


function test_eq_stri32_i32_zero_max() {
  dump("testing: test_eq_stri32_i32_zero_max");
  let a0 = '1';
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


function test_eq_stri32_i32_zero_zero() {
  dump("testing: test_eq_stri32_i32_zero_zero");
  let a0 = '1';
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


function test_eq_stri32_i32_zero_normal() {
  dump("testing: test_eq_stri32_i32_zero_normal");
  let a0 = '1';
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


function test_eq_stri32_i32_zero_min() {
  dump("testing: test_eq_stri32_i32_zero_min");
  let a0 = '1';
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


function test_eq_stri32_i32_normal_max() {
  dump("testing: test_eq_stri32_i32_normal_max");
  let a0 = '1000';
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


function test_eq_stri32_i32_normal_zero() {
  dump("testing: test_eq_stri32_i32_normal_zero");
  let a0 = '1000';
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


function test_eq_stri32_i32_normal_normal() {
  dump("testing: test_eq_stri32_i32_normal_normal");
  let a0 = '1000';
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


function test_eq_stri32_i32_normal_min() {
  dump("testing: test_eq_stri32_i32_normal_min");
  let a0 = '1000';
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


function test_eq_stri32_i32_min_max() {
  dump("testing: test_eq_stri32_i32_min_max");
  let a0 = '-2147483648';
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


function test_eq_stri32_i32_min_zero() {
  dump("testing: test_eq_stri32_i32_min_zero");
  let a0 = '-2147483648';
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


function test_eq_stri32_i32_min_normal() {
  dump("testing: test_eq_stri32_i32_min_normal");
  let a0 = '-2147483648';
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


function test_eq_stri32_i32_min_min() {
  dump("testing: test_eq_stri32_i32_min_min");
  let a0 = '-2147483648';
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


function test_eq_stri32_string_max_max() {
  dump("testing: test_eq_stri32_string_max_max");
  let a0 = '2147483647';
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


function test_eq_stri32_string_max_zero() {
  dump("testing: test_eq_stri32_string_max_zero");
  let a0 = '2147483647';
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


function test_eq_stri32_string_max_normal() {
  dump("testing: test_eq_stri32_string_max_normal");
  let a0 = '2147483647';
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


function test_eq_stri32_string_max_min() {
  dump("testing: test_eq_stri32_string_max_min");
  let a0 = '2147483647';
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


function test_eq_stri32_string_zero_max() {
  dump("testing: test_eq_stri32_string_zero_max");
  let a0 = '1';
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


function test_eq_stri32_string_zero_zero() {
  dump("testing: test_eq_stri32_string_zero_zero");
  let a0 = '1';
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


function test_eq_stri32_string_zero_normal() {
  dump("testing: test_eq_stri32_string_zero_normal");
  let a0 = '1';
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


function test_eq_stri32_string_zero_min() {
  dump("testing: test_eq_stri32_string_zero_min");
  let a0 = '1';
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


function test_eq_stri32_string_normal_max() {
  dump("testing: test_eq_stri32_string_normal_max");
  let a0 = '1000';
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


function test_eq_stri32_string_normal_zero() {
  dump("testing: test_eq_stri32_string_normal_zero");
  let a0 = '1000';
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


function test_eq_stri32_string_normal_normal() {
  dump("testing: test_eq_stri32_string_normal_normal");
  let a0 = '1000';
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


function test_eq_stri32_string_normal_min() {
  dump("testing: test_eq_stri32_string_normal_min");
  let a0 = '1000';
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


function test_eq_stri32_string_min_max() {
  dump("testing: test_eq_stri32_string_min_max");
  let a0 = '-2147483648';
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


function test_eq_stri32_string_min_zero() {
  dump("testing: test_eq_stri32_string_min_zero");
  let a0 = '-2147483648';
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


function test_eq_stri32_string_min_normal() {
  dump("testing: test_eq_stri32_string_min_normal");
  let a0 = '-2147483648';
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


function test_eq_stri32_string_min_min() {
  dump("testing: test_eq_stri32_string_min_min");
  let a0 = '-2147483648';
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


function test_eq_stri32_bool_max_max() {
  dump("testing: test_eq_stri32_bool_max_max");
  let a0 = '2147483647';
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


function test_eq_stri32_bool_max_zero() {
  dump("testing: test_eq_stri32_bool_max_zero");
  let a0 = '2147483647';
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


function test_eq_stri32_bool_max_normal() {
  dump("testing: test_eq_stri32_bool_max_normal");
  let a0 = '2147483647';
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


function test_eq_stri32_bool_max_min() {
  dump("testing: test_eq_stri32_bool_max_min");
  let a0 = '2147483647';
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


function test_eq_stri32_bool_zero_max() {
  dump("testing: test_eq_stri32_bool_zero_max");
  let a0 = '1';
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


function test_eq_stri32_bool_zero_zero() {
  dump("testing: test_eq_stri32_bool_zero_zero");
  let a0 = '1';
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


function test_eq_stri32_bool_zero_normal() {
  dump("testing: test_eq_stri32_bool_zero_normal");
  let a0 = '1';
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


function test_eq_stri32_bool_zero_min() {
  dump("testing: test_eq_stri32_bool_zero_min");
  let a0 = '1';
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


function test_eq_stri32_bool_normal_max() {
  dump("testing: test_eq_stri32_bool_normal_max");
  let a0 = '1000';
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


function test_eq_stri32_bool_normal_zero() {
  dump("testing: test_eq_stri32_bool_normal_zero");
  let a0 = '1000';
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


function test_eq_stri32_bool_normal_normal() {
  dump("testing: test_eq_stri32_bool_normal_normal");
  let a0 = '1000';
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


function test_eq_stri32_bool_normal_min() {
  dump("testing: test_eq_stri32_bool_normal_min");
  let a0 = '1000';
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


function test_eq_stri32_bool_min_max() {
  dump("testing: test_eq_stri32_bool_min_max");
  let a0 = '-2147483648';
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


function test_eq_stri32_bool_min_zero() {
  dump("testing: test_eq_stri32_bool_min_zero");
  let a0 = '-2147483648';
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


function test_eq_stri32_bool_min_normal() {
  dump("testing: test_eq_stri32_bool_min_normal");
  let a0 = '-2147483648';
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


function test_eq_stri32_bool_min_min() {
  dump("testing: test_eq_stri32_bool_min_min");
  let a0 = '-2147483648';
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


function test_eq_stri32_stri32_max_max() {
  dump("testing: test_eq_stri32_stri32_max_max");
  let a0 = '2147483647';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_max_zero() {
  dump("testing: test_eq_stri32_stri32_max_zero");
  let a0 = '2147483647';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_max_normal() {
  dump("testing: test_eq_stri32_stri32_max_normal");
  let a0 = '2147483647';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_max_min() {
  dump("testing: test_eq_stri32_stri32_max_min");
  let a0 = '2147483647';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_zero_max() {
  dump("testing: test_eq_stri32_stri32_zero_max");
  let a0 = '1';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_zero_zero() {
  dump("testing: test_eq_stri32_stri32_zero_zero");
  let a0 = '1';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_zero_normal() {
  dump("testing: test_eq_stri32_stri32_zero_normal");
  let a0 = '1';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_zero_min() {
  dump("testing: test_eq_stri32_stri32_zero_min");
  let a0 = '1';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_normal_max() {
  dump("testing: test_eq_stri32_stri32_normal_max");
  let a0 = '1000';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_normal_zero() {
  dump("testing: test_eq_stri32_stri32_normal_zero");
  let a0 = '1000';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_normal_normal() {
  dump("testing: test_eq_stri32_stri32_normal_normal");
  let a0 = '1000';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_normal_min() {
  dump("testing: test_eq_stri32_stri32_normal_min");
  let a0 = '1000';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_min_max() {
  dump("testing: test_eq_stri32_stri32_min_max");
  let a0 = '-2147483648';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_min_zero() {
  dump("testing: test_eq_stri32_stri32_min_zero");
  let a0 = '-2147483648';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_min_normal() {
  dump("testing: test_eq_stri32_stri32_min_normal");
  let a0 = '-2147483648';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_stri32_min_min() {
  dump("testing: test_eq_stri32_stri32_min_min");
  let a0 = '-2147483648';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_max_max() {
  dump("testing: test_eq_stri32_strf64_max_max");
  let a0 = '2147483647';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_max_zero() {
  dump("testing: test_eq_stri32_strf64_max_zero");
  let a0 = '2147483647';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_max_normal() {
  dump("testing: test_eq_stri32_strf64_max_normal");
  let a0 = '2147483647';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_max_min() {
  dump("testing: test_eq_stri32_strf64_max_min");
  let a0 = '2147483647';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_zero_max() {
  dump("testing: test_eq_stri32_strf64_zero_max");
  let a0 = '1';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_zero_zero() {
  dump("testing: test_eq_stri32_strf64_zero_zero");
  let a0 = '1';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_zero_normal() {
  dump("testing: test_eq_stri32_strf64_zero_normal");
  let a0 = '1';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_zero_min() {
  dump("testing: test_eq_stri32_strf64_zero_min");
  let a0 = '1';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_normal_max() {
  dump("testing: test_eq_stri32_strf64_normal_max");
  let a0 = '1000';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_normal_zero() {
  dump("testing: test_eq_stri32_strf64_normal_zero");
  let a0 = '1000';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_normal_normal() {
  dump("testing: test_eq_stri32_strf64_normal_normal");
  let a0 = '1000';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_normal_min() {
  dump("testing: test_eq_stri32_strf64_normal_min");
  let a0 = '1000';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_min_max() {
  dump("testing: test_eq_stri32_strf64_min_max");
  let a0 = '-2147483648';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_min_zero() {
  dump("testing: test_eq_stri32_strf64_min_zero");
  let a0 = '-2147483648';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_min_normal() {
  dump("testing: test_eq_stri32_strf64_min_normal");
  let a0 = '-2147483648';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_strf64_min_min() {
  dump("testing: test_eq_stri32_strf64_min_min");
  let a0 = '-2147483648';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_stri32_undefined_or_null_max_max() {
  dump("testing: test_eq_stri32_undefined_or_null_max_max");
  let a0 = '2147483647';
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


function test_eq_stri32_undefined_or_null_max_zero() {
  dump("testing: test_eq_stri32_undefined_or_null_max_zero");
  let a0 = '2147483647';
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


function test_eq_stri32_undefined_or_null_max_normal() {
  dump("testing: test_eq_stri32_undefined_or_null_max_normal");
  let a0 = '2147483647';
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


function test_eq_stri32_undefined_or_null_max_min() {
  dump("testing: test_eq_stri32_undefined_or_null_max_min");
  let a0 = '2147483647';
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


function test_eq_stri32_undefined_or_null_zero_max() {
  dump("testing: test_eq_stri32_undefined_or_null_zero_max");
  let a0 = '1';
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


function test_eq_stri32_undefined_or_null_zero_zero() {
  dump("testing: test_eq_stri32_undefined_or_null_zero_zero");
  let a0 = '1';
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


function test_eq_stri32_undefined_or_null_zero_normal() {
  dump("testing: test_eq_stri32_undefined_or_null_zero_normal");
  let a0 = '1';
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


function test_eq_stri32_undefined_or_null_zero_min() {
  dump("testing: test_eq_stri32_undefined_or_null_zero_min");
  let a0 = '1';
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


function test_eq_stri32_undefined_or_null_normal_max() {
  dump("testing: test_eq_stri32_undefined_or_null_normal_max");
  let a0 = '1000';
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


function test_eq_stri32_undefined_or_null_normal_zero() {
  dump("testing: test_eq_stri32_undefined_or_null_normal_zero");
  let a0 = '1000';
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


function test_eq_stri32_undefined_or_null_normal_normal() {
  dump("testing: test_eq_stri32_undefined_or_null_normal_normal");
  let a0 = '1000';
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


function test_eq_stri32_undefined_or_null_normal_min() {
  dump("testing: test_eq_stri32_undefined_or_null_normal_min");
  let a0 = '1000';
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


function test_eq_stri32_undefined_or_null_min_max() {
  dump("testing: test_eq_stri32_undefined_or_null_min_max");
  let a0 = '-2147483648';
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


function test_eq_stri32_undefined_or_null_min_zero() {
  dump("testing: test_eq_stri32_undefined_or_null_min_zero");
  let a0 = '-2147483648';
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


function test_eq_stri32_undefined_or_null_min_normal() {
  dump("testing: test_eq_stri32_undefined_or_null_min_normal");
  let a0 = '-2147483648';
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


function test_eq_stri32_undefined_or_null_min_min() {
  dump("testing: test_eq_stri32_undefined_or_null_min_min");
  let a0 = '-2147483648';
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


function test_eq_strf64_f64_max_normal() {
  dump("testing: test_eq_strf64_f64_max_normal");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_f64_max_max() {
  dump("testing: test_eq_strf64_f64_max_max");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_f64_max_min() {
  dump("testing: test_eq_strf64_f64_max_min");
  let a0 = '1.79769313486231570e+308';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_max_nan() {
  dump("testing: test_eq_strf64_f64_max_nan");
  let a0 = '1.79769313486231570e+308';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_max_pinf() {
  dump("testing: test_eq_strf64_f64_max_pinf");
  let a0 = '1.79769313486231570e+308';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_max_zero() {
  dump("testing: test_eq_strf64_f64_max_zero");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_f64_max_ninf() {
  dump("testing: test_eq_strf64_f64_max_ninf");
  let a0 = '1.79769313486231570e+308';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_zero_normal() {
  dump("testing: test_eq_strf64_f64_zero_normal");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_f64_zero_max() {
  dump("testing: test_eq_strf64_f64_zero_max");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_f64_zero_min() {
  dump("testing: test_eq_strf64_f64_zero_min");
  let a0 = '2.2250738585072014E-308';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_zero_nan() {
  dump("testing: test_eq_strf64_f64_zero_nan");
  let a0 = '2.2250738585072014E-308';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_zero_pinf() {
  dump("testing: test_eq_strf64_f64_zero_pinf");
  let a0 = '2.2250738585072014E-308';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_zero_zero() {
  dump("testing: test_eq_strf64_f64_zero_zero");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_f64_zero_ninf() {
  dump("testing: test_eq_strf64_f64_zero_ninf");
  let a0 = '2.2250738585072014E-308';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_normal_normal() {
  dump("testing: test_eq_strf64_f64_normal_normal");
  let a0 = '13.2';
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


function test_eq_strf64_f64_normal_max() {
  dump("testing: test_eq_strf64_f64_normal_max");
  let a0 = '13.2';
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


function test_eq_strf64_f64_normal_min() {
  dump("testing: test_eq_strf64_f64_normal_min");
  let a0 = '13.2';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_normal_nan() {
  dump("testing: test_eq_strf64_f64_normal_nan");
  let a0 = '13.2';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_normal_pinf() {
  dump("testing: test_eq_strf64_f64_normal_pinf");
  let a0 = '13.2';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_normal_zero() {
  dump("testing: test_eq_strf64_f64_normal_zero");
  let a0 = '13.2';
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


function test_eq_strf64_f64_normal_ninf() {
  dump("testing: test_eq_strf64_f64_normal_ninf");
  let a0 = '13.2';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_min_normal() {
  dump("testing: test_eq_strf64_f64_min_normal");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_f64_min_max() {
  dump("testing: test_eq_strf64_f64_min_max");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_f64_min_min() {
  dump("testing: test_eq_strf64_f64_min_min");
  let a0 = '-2.2250738585072014E-308';
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_min_nan() {
  dump("testing: test_eq_strf64_f64_min_nan");
  let a0 = '-2.2250738585072014E-308';
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_min_pinf() {
  dump("testing: test_eq_strf64_f64_min_pinf");
  let a0 = '-2.2250738585072014E-308';
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_f64_min_zero() {
  dump("testing: test_eq_strf64_f64_min_zero");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_f64_min_ninf() {
  dump("testing: test_eq_strf64_f64_min_ninf");
  let a0 = '-2.2250738585072014E-308';
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_i32_max_max() {
  dump("testing: test_eq_strf64_i32_max_max");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_i32_max_zero() {
  dump("testing: test_eq_strf64_i32_max_zero");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_i32_max_normal() {
  dump("testing: test_eq_strf64_i32_max_normal");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_i32_max_min() {
  dump("testing: test_eq_strf64_i32_max_min");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_i32_zero_max() {
  dump("testing: test_eq_strf64_i32_zero_max");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_i32_zero_zero() {
  dump("testing: test_eq_strf64_i32_zero_zero");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_i32_zero_normal() {
  dump("testing: test_eq_strf64_i32_zero_normal");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_i32_zero_min() {
  dump("testing: test_eq_strf64_i32_zero_min");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_i32_normal_max() {
  dump("testing: test_eq_strf64_i32_normal_max");
  let a0 = '13.2';
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


function test_eq_strf64_i32_normal_zero() {
  dump("testing: test_eq_strf64_i32_normal_zero");
  let a0 = '13.2';
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


function test_eq_strf64_i32_normal_normal() {
  dump("testing: test_eq_strf64_i32_normal_normal");
  let a0 = '13.2';
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


function test_eq_strf64_i32_normal_min() {
  dump("testing: test_eq_strf64_i32_normal_min");
  let a0 = '13.2';
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


function test_eq_strf64_i32_min_max() {
  dump("testing: test_eq_strf64_i32_min_max");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_i32_min_zero() {
  dump("testing: test_eq_strf64_i32_min_zero");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_i32_min_normal() {
  dump("testing: test_eq_strf64_i32_min_normal");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_i32_min_min() {
  dump("testing: test_eq_strf64_i32_min_min");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_string_max_max() {
  dump("testing: test_eq_strf64_string_max_max");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_string_max_zero() {
  dump("testing: test_eq_strf64_string_max_zero");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_string_max_normal() {
  dump("testing: test_eq_strf64_string_max_normal");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_string_max_min() {
  dump("testing: test_eq_strf64_string_max_min");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_string_zero_max() {
  dump("testing: test_eq_strf64_string_zero_max");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_string_zero_zero() {
  dump("testing: test_eq_strf64_string_zero_zero");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_string_zero_normal() {
  dump("testing: test_eq_strf64_string_zero_normal");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_string_zero_min() {
  dump("testing: test_eq_strf64_string_zero_min");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_string_normal_max() {
  dump("testing: test_eq_strf64_string_normal_max");
  let a0 = '13.2';
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


function test_eq_strf64_string_normal_zero() {
  dump("testing: test_eq_strf64_string_normal_zero");
  let a0 = '13.2';
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


function test_eq_strf64_string_normal_normal() {
  dump("testing: test_eq_strf64_string_normal_normal");
  let a0 = '13.2';
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


function test_eq_strf64_string_normal_min() {
  dump("testing: test_eq_strf64_string_normal_min");
  let a0 = '13.2';
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


function test_eq_strf64_string_min_max() {
  dump("testing: test_eq_strf64_string_min_max");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_string_min_zero() {
  dump("testing: test_eq_strf64_string_min_zero");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_string_min_normal() {
  dump("testing: test_eq_strf64_string_min_normal");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_string_min_min() {
  dump("testing: test_eq_strf64_string_min_min");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_bool_max_max() {
  dump("testing: test_eq_strf64_bool_max_max");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_bool_max_zero() {
  dump("testing: test_eq_strf64_bool_max_zero");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_bool_max_normal() {
  dump("testing: test_eq_strf64_bool_max_normal");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_bool_max_min() {
  dump("testing: test_eq_strf64_bool_max_min");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_bool_zero_max() {
  dump("testing: test_eq_strf64_bool_zero_max");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_bool_zero_zero() {
  dump("testing: test_eq_strf64_bool_zero_zero");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_bool_zero_normal() {
  dump("testing: test_eq_strf64_bool_zero_normal");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_bool_zero_min() {
  dump("testing: test_eq_strf64_bool_zero_min");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_bool_normal_max() {
  dump("testing: test_eq_strf64_bool_normal_max");
  let a0 = '13.2';
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


function test_eq_strf64_bool_normal_zero() {
  dump("testing: test_eq_strf64_bool_normal_zero");
  let a0 = '13.2';
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


function test_eq_strf64_bool_normal_normal() {
  dump("testing: test_eq_strf64_bool_normal_normal");
  let a0 = '13.2';
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


function test_eq_strf64_bool_normal_min() {
  dump("testing: test_eq_strf64_bool_normal_min");
  let a0 = '13.2';
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


function test_eq_strf64_bool_min_max() {
  dump("testing: test_eq_strf64_bool_min_max");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_bool_min_zero() {
  dump("testing: test_eq_strf64_bool_min_zero");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_bool_min_normal() {
  dump("testing: test_eq_strf64_bool_min_normal");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_bool_min_min() {
  dump("testing: test_eq_strf64_bool_min_min");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_stri32_max_max() {
  dump("testing: test_eq_strf64_stri32_max_max");
  let a0 = '1.79769313486231570e+308';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_max_zero() {
  dump("testing: test_eq_strf64_stri32_max_zero");
  let a0 = '1.79769313486231570e+308';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_max_normal() {
  dump("testing: test_eq_strf64_stri32_max_normal");
  let a0 = '1.79769313486231570e+308';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_max_min() {
  dump("testing: test_eq_strf64_stri32_max_min");
  let a0 = '1.79769313486231570e+308';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_zero_max() {
  dump("testing: test_eq_strf64_stri32_zero_max");
  let a0 = '2.2250738585072014E-308';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_zero_zero() {
  dump("testing: test_eq_strf64_stri32_zero_zero");
  let a0 = '2.2250738585072014E-308';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_zero_normal() {
  dump("testing: test_eq_strf64_stri32_zero_normal");
  let a0 = '2.2250738585072014E-308';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_zero_min() {
  dump("testing: test_eq_strf64_stri32_zero_min");
  let a0 = '2.2250738585072014E-308';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_normal_max() {
  dump("testing: test_eq_strf64_stri32_normal_max");
  let a0 = '13.2';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_normal_zero() {
  dump("testing: test_eq_strf64_stri32_normal_zero");
  let a0 = '13.2';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_normal_normal() {
  dump("testing: test_eq_strf64_stri32_normal_normal");
  let a0 = '13.2';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_normal_min() {
  dump("testing: test_eq_strf64_stri32_normal_min");
  let a0 = '13.2';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_min_max() {
  dump("testing: test_eq_strf64_stri32_min_max");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_min_zero() {
  dump("testing: test_eq_strf64_stri32_min_zero");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_min_normal() {
  dump("testing: test_eq_strf64_stri32_min_normal");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_stri32_min_min() {
  dump("testing: test_eq_strf64_stri32_min_min");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_max_max() {
  dump("testing: test_eq_strf64_strf64_max_max");
  let a0 = '1.79769313486231570e+308';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_max_zero() {
  dump("testing: test_eq_strf64_strf64_max_zero");
  let a0 = '1.79769313486231570e+308';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_max_normal() {
  dump("testing: test_eq_strf64_strf64_max_normal");
  let a0 = '1.79769313486231570e+308';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_max_min() {
  dump("testing: test_eq_strf64_strf64_max_min");
  let a0 = '1.79769313486231570e+308';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_zero_max() {
  dump("testing: test_eq_strf64_strf64_zero_max");
  let a0 = '2.2250738585072014E-308';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_zero_zero() {
  dump("testing: test_eq_strf64_strf64_zero_zero");
  let a0 = '2.2250738585072014E-308';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_zero_normal() {
  dump("testing: test_eq_strf64_strf64_zero_normal");
  let a0 = '2.2250738585072014E-308';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_zero_min() {
  dump("testing: test_eq_strf64_strf64_zero_min");
  let a0 = '2.2250738585072014E-308';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_normal_max() {
  dump("testing: test_eq_strf64_strf64_normal_max");
  let a0 = '13.2';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_normal_zero() {
  dump("testing: test_eq_strf64_strf64_normal_zero");
  let a0 = '13.2';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_normal_normal() {
  dump("testing: test_eq_strf64_strf64_normal_normal");
  let a0 = '13.2';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_normal_min() {
  dump("testing: test_eq_strf64_strf64_normal_min");
  let a0 = '13.2';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_min_max() {
  dump("testing: test_eq_strf64_strf64_min_max");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_min_zero() {
  dump("testing: test_eq_strf64_strf64_min_zero");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_min_normal() {
  dump("testing: test_eq_strf64_strf64_min_normal");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_strf64_min_min() {
  dump("testing: test_eq_strf64_strf64_min_min");
  let a0 = '-2.2250738585072014E-308';
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_strf64_undefined_or_null_max_max() {
  dump("testing: test_eq_strf64_undefined_or_null_max_max");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_undefined_or_null_max_zero() {
  dump("testing: test_eq_strf64_undefined_or_null_max_zero");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_undefined_or_null_max_normal() {
  dump("testing: test_eq_strf64_undefined_or_null_max_normal");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_undefined_or_null_max_min() {
  dump("testing: test_eq_strf64_undefined_or_null_max_min");
  let a0 = '1.79769313486231570e+308';
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


function test_eq_strf64_undefined_or_null_zero_max() {
  dump("testing: test_eq_strf64_undefined_or_null_zero_max");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_zero_zero() {
  dump("testing: test_eq_strf64_undefined_or_null_zero_zero");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_zero_normal() {
  dump("testing: test_eq_strf64_undefined_or_null_zero_normal");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_zero_min() {
  dump("testing: test_eq_strf64_undefined_or_null_zero_min");
  let a0 = '2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_normal_max() {
  dump("testing: test_eq_strf64_undefined_or_null_normal_max");
  let a0 = '13.2';
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


function test_eq_strf64_undefined_or_null_normal_zero() {
  dump("testing: test_eq_strf64_undefined_or_null_normal_zero");
  let a0 = '13.2';
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


function test_eq_strf64_undefined_or_null_normal_normal() {
  dump("testing: test_eq_strf64_undefined_or_null_normal_normal");
  let a0 = '13.2';
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


function test_eq_strf64_undefined_or_null_normal_min() {
  dump("testing: test_eq_strf64_undefined_or_null_normal_min");
  let a0 = '13.2';
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


function test_eq_strf64_undefined_or_null_min_max() {
  dump("testing: test_eq_strf64_undefined_or_null_min_max");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_min_zero() {
  dump("testing: test_eq_strf64_undefined_or_null_min_zero");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_min_normal() {
  dump("testing: test_eq_strf64_undefined_or_null_min_normal");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_strf64_undefined_or_null_min_min() {
  dump("testing: test_eq_strf64_undefined_or_null_min_min");
  let a0 = '-2.2250738585072014E-308';
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


function test_eq_undefined_or_null_f64_max_max() {
  dump("testing: test_eq_undefined_or_null_f64_max_max");
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


function test_eq_undefined_or_null_f64_max_min() {
  dump("testing: test_eq_undefined_or_null_f64_max_min");
  let a0 = null;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_nan() {
  dump("testing: test_eq_undefined_or_null_f64_max_nan");
  let a0 = null;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_max_pinf() {
  dump("testing: test_eq_undefined_or_null_f64_max_pinf");
  let a0 = null;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_max_ninf() {
  dump("testing: test_eq_undefined_or_null_f64_max_ninf");
  let a0 = null;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_zero_max() {
  dump("testing: test_eq_undefined_or_null_f64_zero_max");
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


function test_eq_undefined_or_null_f64_zero_min() {
  dump("testing: test_eq_undefined_or_null_f64_zero_min");
  let a0 = undefined;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_nan() {
  dump("testing: test_eq_undefined_or_null_f64_zero_nan");
  let a0 = undefined;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_zero_pinf() {
  dump("testing: test_eq_undefined_or_null_f64_zero_pinf");
  let a0 = undefined;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_zero_ninf() {
  dump("testing: test_eq_undefined_or_null_f64_zero_ninf");
  let a0 = undefined;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_normal_max() {
  dump("testing: test_eq_undefined_or_null_f64_normal_max");
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


function test_eq_undefined_or_null_f64_normal_min() {
  dump("testing: test_eq_undefined_or_null_f64_normal_min");
  let a0 = null;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_nan() {
  dump("testing: test_eq_undefined_or_null_f64_normal_nan");
  let a0 = null;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_normal_pinf() {
  dump("testing: test_eq_undefined_or_null_f64_normal_pinf");
  let a0 = null;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_normal_ninf() {
  dump("testing: test_eq_undefined_or_null_f64_normal_ninf");
  let a0 = null;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_min_max() {
  dump("testing: test_eq_undefined_or_null_f64_min_max");
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


function test_eq_undefined_or_null_f64_min_min() {
  dump("testing: test_eq_undefined_or_null_f64_min_min");
  let a0 = undefined;
  let a1 = -1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_nan() {
  dump("testing: test_eq_undefined_or_null_f64_min_nan");
  let a0 = undefined;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_f64_min_pinf() {
  dump("testing: test_eq_undefined_or_null_f64_min_pinf");
  let a0 = undefined;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_f64_min_ninf() {
  dump("testing: test_eq_undefined_or_null_f64_min_ninf");
  let a0 = undefined;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 == a1;
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


function test_eq_undefined_or_null_stri32_max_max() {
  dump("testing: test_eq_undefined_or_null_stri32_max_max");
  let a0 = null;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_max_zero() {
  dump("testing: test_eq_undefined_or_null_stri32_max_zero");
  let a0 = null;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_max_normal() {
  dump("testing: test_eq_undefined_or_null_stri32_max_normal");
  let a0 = null;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_max_min() {
  dump("testing: test_eq_undefined_or_null_stri32_max_min");
  let a0 = null;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_zero_max() {
  dump("testing: test_eq_undefined_or_null_stri32_zero_max");
  let a0 = undefined;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_zero_zero() {
  dump("testing: test_eq_undefined_or_null_stri32_zero_zero");
  let a0 = undefined;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_zero_normal() {
  dump("testing: test_eq_undefined_or_null_stri32_zero_normal");
  let a0 = undefined;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_zero_min() {
  dump("testing: test_eq_undefined_or_null_stri32_zero_min");
  let a0 = undefined;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_normal_max() {
  dump("testing: test_eq_undefined_or_null_stri32_normal_max");
  let a0 = null;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_normal_zero() {
  dump("testing: test_eq_undefined_or_null_stri32_normal_zero");
  let a0 = null;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_normal_normal() {
  dump("testing: test_eq_undefined_or_null_stri32_normal_normal");
  let a0 = null;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_normal_min() {
  dump("testing: test_eq_undefined_or_null_stri32_normal_min");
  let a0 = null;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_min_max() {
  dump("testing: test_eq_undefined_or_null_stri32_min_max");
  let a0 = undefined;
  let a1 = '2147483647';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_min_zero() {
  dump("testing: test_eq_undefined_or_null_stri32_min_zero");
  let a0 = undefined;
  let a1 = '1';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_min_normal() {
  dump("testing: test_eq_undefined_or_null_stri32_min_normal");
  let a0 = undefined;
  let a1 = '1000';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_stri32_min_min() {
  dump("testing: test_eq_undefined_or_null_stri32_min_min");
  let a0 = undefined;
  let a1 = '-2147483648';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_max_max() {
  dump("testing: test_eq_undefined_or_null_strf64_max_max");
  let a0 = null;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_max_zero() {
  dump("testing: test_eq_undefined_or_null_strf64_max_zero");
  let a0 = null;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_max_normal() {
  dump("testing: test_eq_undefined_or_null_strf64_max_normal");
  let a0 = null;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_max_min() {
  dump("testing: test_eq_undefined_or_null_strf64_max_min");
  let a0 = null;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_zero_max() {
  dump("testing: test_eq_undefined_or_null_strf64_zero_max");
  let a0 = undefined;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_zero_zero() {
  dump("testing: test_eq_undefined_or_null_strf64_zero_zero");
  let a0 = undefined;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_zero_normal() {
  dump("testing: test_eq_undefined_or_null_strf64_zero_normal");
  let a0 = undefined;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_zero_min() {
  dump("testing: test_eq_undefined_or_null_strf64_zero_min");
  let a0 = undefined;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_normal_max() {
  dump("testing: test_eq_undefined_or_null_strf64_normal_max");
  let a0 = null;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_normal_zero() {
  dump("testing: test_eq_undefined_or_null_strf64_normal_zero");
  let a0 = null;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_normal_normal() {
  dump("testing: test_eq_undefined_or_null_strf64_normal_normal");
  let a0 = null;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_normal_min() {
  dump("testing: test_eq_undefined_or_null_strf64_normal_min");
  let a0 = null;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_min_max() {
  dump("testing: test_eq_undefined_or_null_strf64_min_max");
  let a0 = undefined;
  let a1 = '1.79769313486231570e+308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_min_zero() {
  dump("testing: test_eq_undefined_or_null_strf64_min_zero");
  let a0 = undefined;
  let a1 = '2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_min_normal() {
  dump("testing: test_eq_undefined_or_null_strf64_min_normal");
  let a0 = undefined;
  let a1 = '13.2';
  let test_value = undefined;
  let expect = a0 == a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 == a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_eq_undefined_or_null_strf64_min_min() {
  dump("testing: test_eq_undefined_or_null_strf64_min_min");
  let a0 = undefined;
  let a1 = '-2.2250738585072014E-308';
  let test_value = undefined;
  let expect = a0 == a1;
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
  test_eq_f64_f64_normal_normal();
  test_eq_f64_f64_normal_max();
  test_eq_f64_f64_normal_min();
  test_eq_f64_f64_normal_nan();
  test_eq_f64_f64_normal_pinf();
  test_eq_f64_f64_normal_zero();
  test_eq_f64_f64_normal_ninf();
  test_eq_f64_f64_max_normal();
  test_eq_f64_f64_max_max();
  test_eq_f64_f64_max_min();
  test_eq_f64_f64_max_nan();
  test_eq_f64_f64_max_pinf();
  test_eq_f64_f64_max_zero();
  test_eq_f64_f64_max_ninf();
  test_eq_f64_f64_min_normal();
  test_eq_f64_f64_min_max();
  test_eq_f64_f64_min_min();
  test_eq_f64_f64_min_nan();
  test_eq_f64_f64_min_pinf();
  test_eq_f64_f64_min_zero();
  test_eq_f64_f64_min_ninf();
  test_eq_f64_f64_nan_normal();
  test_eq_f64_f64_nan_max();
  test_eq_f64_f64_nan_min();
  test_eq_f64_f64_nan_nan();
  test_eq_f64_f64_nan_pinf();
  test_eq_f64_f64_nan_zero();
  test_eq_f64_f64_nan_ninf();
  test_eq_f64_f64_pinf_normal();
  test_eq_f64_f64_pinf_max();
  test_eq_f64_f64_pinf_min();
  test_eq_f64_f64_pinf_nan();
  test_eq_f64_f64_pinf_pinf();
  test_eq_f64_f64_pinf_zero();
  test_eq_f64_f64_pinf_ninf();
  test_eq_f64_f64_zero_normal();
  test_eq_f64_f64_zero_max();
  test_eq_f64_f64_zero_min();
  test_eq_f64_f64_zero_nan();
  test_eq_f64_f64_zero_pinf();
  test_eq_f64_f64_zero_zero();
  test_eq_f64_f64_zero_ninf();
  test_eq_f64_f64_ninf_normal();
  test_eq_f64_f64_ninf_max();
  test_eq_f64_f64_ninf_min();
  test_eq_f64_f64_ninf_nan();
  test_eq_f64_f64_ninf_pinf();
  test_eq_f64_f64_ninf_zero();
  test_eq_f64_f64_ninf_ninf();
  test_eq_f64_i32_normal_max();
  test_eq_f64_i32_normal_zero();
  test_eq_f64_i32_normal_normal();
  test_eq_f64_i32_normal_min();
  test_eq_f64_i32_max_max();
  test_eq_f64_i32_max_zero();
  test_eq_f64_i32_max_normal();
  test_eq_f64_i32_max_min();
  test_eq_f64_i32_min_max();
  test_eq_f64_i32_min_zero();
  test_eq_f64_i32_min_normal();
  test_eq_f64_i32_min_min();
  test_eq_f64_i32_nan_max();
  test_eq_f64_i32_nan_zero();
  test_eq_f64_i32_nan_normal();
  test_eq_f64_i32_nan_min();
  test_eq_f64_i32_pinf_max();
  test_eq_f64_i32_pinf_zero();
  test_eq_f64_i32_pinf_normal();
  test_eq_f64_i32_pinf_min();
  test_eq_f64_i32_zero_max();
  test_eq_f64_i32_zero_zero();
  test_eq_f64_i32_zero_normal();
  test_eq_f64_i32_zero_min();
  test_eq_f64_i32_ninf_max();
  test_eq_f64_i32_ninf_zero();
  test_eq_f64_i32_ninf_normal();
  test_eq_f64_i32_ninf_min();
  test_eq_f64_string_normal_max();
  test_eq_f64_string_normal_zero();
  test_eq_f64_string_normal_normal();
  test_eq_f64_string_normal_min();
  test_eq_f64_string_max_max();
  test_eq_f64_string_max_zero();
  test_eq_f64_string_max_normal();
  test_eq_f64_string_max_min();
  test_eq_f64_string_min_max();
  test_eq_f64_string_min_zero();
  test_eq_f64_string_min_normal();
  test_eq_f64_string_min_min();
  test_eq_f64_string_nan_max();
  test_eq_f64_string_nan_zero();
  test_eq_f64_string_nan_normal();
  test_eq_f64_string_nan_min();
  test_eq_f64_string_pinf_max();
  test_eq_f64_string_pinf_zero();
  test_eq_f64_string_pinf_normal();
  test_eq_f64_string_pinf_min();
  test_eq_f64_string_zero_max();
  test_eq_f64_string_zero_zero();
  test_eq_f64_string_zero_normal();
  test_eq_f64_string_zero_min();
  test_eq_f64_string_ninf_max();
  test_eq_f64_string_ninf_zero();
  test_eq_f64_string_ninf_normal();
  test_eq_f64_string_ninf_min();
  test_eq_f64_bool_normal_max();
  test_eq_f64_bool_normal_zero();
  test_eq_f64_bool_normal_normal();
  test_eq_f64_bool_normal_min();
  test_eq_f64_bool_max_max();
  test_eq_f64_bool_max_zero();
  test_eq_f64_bool_max_normal();
  test_eq_f64_bool_max_min();
  test_eq_f64_bool_min_max();
  test_eq_f64_bool_min_zero();
  test_eq_f64_bool_min_normal();
  test_eq_f64_bool_min_min();
  test_eq_f64_bool_nan_max();
  test_eq_f64_bool_nan_zero();
  test_eq_f64_bool_nan_normal();
  test_eq_f64_bool_nan_min();
  test_eq_f64_bool_pinf_max();
  test_eq_f64_bool_pinf_zero();
  test_eq_f64_bool_pinf_normal();
  test_eq_f64_bool_pinf_min();
  test_eq_f64_bool_zero_max();
  test_eq_f64_bool_zero_zero();
  test_eq_f64_bool_zero_normal();
  test_eq_f64_bool_zero_min();
  test_eq_f64_bool_ninf_max();
  test_eq_f64_bool_ninf_zero();
  test_eq_f64_bool_ninf_normal();
  test_eq_f64_bool_ninf_min();
  test_eq_f64_stri32_normal_max();
  test_eq_f64_stri32_normal_zero();
  test_eq_f64_stri32_normal_normal();
  test_eq_f64_stri32_normal_min();
  test_eq_f64_stri32_max_max();
  test_eq_f64_stri32_max_zero();
  test_eq_f64_stri32_max_normal();
  test_eq_f64_stri32_max_min();
  test_eq_f64_stri32_min_max();
  test_eq_f64_stri32_min_zero();
  test_eq_f64_stri32_min_normal();
  test_eq_f64_stri32_min_min();
  test_eq_f64_stri32_nan_max();
  test_eq_f64_stri32_nan_zero();
  test_eq_f64_stri32_nan_normal();
  test_eq_f64_stri32_nan_min();
  test_eq_f64_stri32_pinf_max();
  test_eq_f64_stri32_pinf_zero();
  test_eq_f64_stri32_pinf_normal();
  test_eq_f64_stri32_pinf_min();
  test_eq_f64_stri32_zero_max();
  test_eq_f64_stri32_zero_zero();
  test_eq_f64_stri32_zero_normal();
  test_eq_f64_stri32_zero_min();
  test_eq_f64_stri32_ninf_max();
  test_eq_f64_stri32_ninf_zero();
  test_eq_f64_stri32_ninf_normal();
  test_eq_f64_stri32_ninf_min();
  test_eq_f64_strf64_normal_max();
  test_eq_f64_strf64_normal_zero();
  test_eq_f64_strf64_normal_normal();
  test_eq_f64_strf64_normal_min();
  test_eq_f64_strf64_max_max();
  test_eq_f64_strf64_max_zero();
  test_eq_f64_strf64_max_normal();
  test_eq_f64_strf64_max_min();
  test_eq_f64_strf64_min_max();
  test_eq_f64_strf64_min_zero();
  test_eq_f64_strf64_min_normal();
  test_eq_f64_strf64_min_min();
  test_eq_f64_strf64_nan_max();
  test_eq_f64_strf64_nan_zero();
  test_eq_f64_strf64_nan_normal();
  test_eq_f64_strf64_nan_min();
  test_eq_f64_strf64_pinf_max();
  test_eq_f64_strf64_pinf_zero();
  test_eq_f64_strf64_pinf_normal();
  test_eq_f64_strf64_pinf_min();
  test_eq_f64_strf64_zero_max();
  test_eq_f64_strf64_zero_zero();
  test_eq_f64_strf64_zero_normal();
  test_eq_f64_strf64_zero_min();
  test_eq_f64_strf64_ninf_max();
  test_eq_f64_strf64_ninf_zero();
  test_eq_f64_strf64_ninf_normal();
  test_eq_f64_strf64_ninf_min();
  test_eq_f64_undefined_or_null_normal_max();
  test_eq_f64_undefined_or_null_normal_zero();
  test_eq_f64_undefined_or_null_normal_normal();
  test_eq_f64_undefined_or_null_normal_min();
  test_eq_f64_undefined_or_null_max_max();
  test_eq_f64_undefined_or_null_max_zero();
  test_eq_f64_undefined_or_null_max_normal();
  test_eq_f64_undefined_or_null_max_min();
  test_eq_f64_undefined_or_null_min_max();
  test_eq_f64_undefined_or_null_min_zero();
  test_eq_f64_undefined_or_null_min_normal();
  test_eq_f64_undefined_or_null_min_min();
  test_eq_f64_undefined_or_null_nan_max();
  test_eq_f64_undefined_or_null_nan_zero();
  test_eq_f64_undefined_or_null_nan_normal();
  test_eq_f64_undefined_or_null_nan_min();
  test_eq_f64_undefined_or_null_pinf_max();
  test_eq_f64_undefined_or_null_pinf_zero();
  test_eq_f64_undefined_or_null_pinf_normal();
  test_eq_f64_undefined_or_null_pinf_min();
  test_eq_f64_undefined_or_null_zero_max();
  test_eq_f64_undefined_or_null_zero_zero();
  test_eq_f64_undefined_or_null_zero_normal();
  test_eq_f64_undefined_or_null_zero_min();
  test_eq_f64_undefined_or_null_ninf_max();
  test_eq_f64_undefined_or_null_ninf_zero();
  test_eq_f64_undefined_or_null_ninf_normal();
  test_eq_f64_undefined_or_null_ninf_min();
  test_eq_i32_f64_max_normal();
  test_eq_i32_f64_max_max();
  test_eq_i32_f64_max_min();
  test_eq_i32_f64_max_nan();
  test_eq_i32_f64_max_pinf();
  test_eq_i32_f64_max_zero();
  test_eq_i32_f64_max_ninf();
  test_eq_i32_f64_zero_normal();
  test_eq_i32_f64_zero_max();
  test_eq_i32_f64_zero_min();
  test_eq_i32_f64_zero_nan();
  test_eq_i32_f64_zero_pinf();
  test_eq_i32_f64_zero_zero();
  test_eq_i32_f64_zero_ninf();
  test_eq_i32_f64_normal_normal();
  test_eq_i32_f64_normal_max();
  test_eq_i32_f64_normal_min();
  test_eq_i32_f64_normal_nan();
  test_eq_i32_f64_normal_pinf();
  test_eq_i32_f64_normal_zero();
  test_eq_i32_f64_normal_ninf();
  test_eq_i32_f64_min_normal();
  test_eq_i32_f64_min_max();
  test_eq_i32_f64_min_min();
  test_eq_i32_f64_min_nan();
  test_eq_i32_f64_min_pinf();
  test_eq_i32_f64_min_zero();
  test_eq_i32_f64_min_ninf();
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
  test_eq_i32_stri32_max_max();
  test_eq_i32_stri32_max_zero();
  test_eq_i32_stri32_max_normal();
  test_eq_i32_stri32_max_min();
  test_eq_i32_stri32_zero_max();
  test_eq_i32_stri32_zero_zero();
  test_eq_i32_stri32_zero_normal();
  test_eq_i32_stri32_zero_min();
  test_eq_i32_stri32_normal_max();
  test_eq_i32_stri32_normal_zero();
  test_eq_i32_stri32_normal_normal();
  test_eq_i32_stri32_normal_min();
  test_eq_i32_stri32_min_max();
  test_eq_i32_stri32_min_zero();
  test_eq_i32_stri32_min_normal();
  test_eq_i32_stri32_min_min();
  test_eq_i32_strf64_max_max();
  test_eq_i32_strf64_max_zero();
  test_eq_i32_strf64_max_normal();
  test_eq_i32_strf64_max_min();
  test_eq_i32_strf64_zero_max();
  test_eq_i32_strf64_zero_zero();
  test_eq_i32_strf64_zero_normal();
  test_eq_i32_strf64_zero_min();
  test_eq_i32_strf64_normal_max();
  test_eq_i32_strf64_normal_zero();
  test_eq_i32_strf64_normal_normal();
  test_eq_i32_strf64_normal_min();
  test_eq_i32_strf64_min_max();
  test_eq_i32_strf64_min_zero();
  test_eq_i32_strf64_min_normal();
  test_eq_i32_strf64_min_min();
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
  test_eq_string_f64_max_normal();
  test_eq_string_f64_max_max();
  test_eq_string_f64_max_min();
  test_eq_string_f64_max_nan();
  test_eq_string_f64_max_pinf();
  test_eq_string_f64_max_zero();
  test_eq_string_f64_max_ninf();
  test_eq_string_f64_zero_normal();
  test_eq_string_f64_zero_max();
  test_eq_string_f64_zero_min();
  test_eq_string_f64_zero_nan();
  test_eq_string_f64_zero_pinf();
  test_eq_string_f64_zero_zero();
  test_eq_string_f64_zero_ninf();
  test_eq_string_f64_normal_normal();
  test_eq_string_f64_normal_max();
  test_eq_string_f64_normal_min();
  test_eq_string_f64_normal_nan();
  test_eq_string_f64_normal_pinf();
  test_eq_string_f64_normal_zero();
  test_eq_string_f64_normal_ninf();
  test_eq_string_f64_min_normal();
  test_eq_string_f64_min_max();
  test_eq_string_f64_min_min();
  test_eq_string_f64_min_nan();
  test_eq_string_f64_min_pinf();
  test_eq_string_f64_min_zero();
  test_eq_string_f64_min_ninf();
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
  test_eq_string_stri32_max_max();
  test_eq_string_stri32_max_zero();
  test_eq_string_stri32_max_normal();
  test_eq_string_stri32_max_min();
  test_eq_string_stri32_zero_max();
  test_eq_string_stri32_zero_zero();
  test_eq_string_stri32_zero_normal();
  test_eq_string_stri32_zero_min();
  test_eq_string_stri32_normal_max();
  test_eq_string_stri32_normal_zero();
  test_eq_string_stri32_normal_normal();
  test_eq_string_stri32_normal_min();
  test_eq_string_stri32_min_max();
  test_eq_string_stri32_min_zero();
  test_eq_string_stri32_min_normal();
  test_eq_string_stri32_min_min();
  test_eq_string_strf64_max_max();
  test_eq_string_strf64_max_zero();
  test_eq_string_strf64_max_normal();
  test_eq_string_strf64_max_min();
  test_eq_string_strf64_zero_max();
  test_eq_string_strf64_zero_zero();
  test_eq_string_strf64_zero_normal();
  test_eq_string_strf64_zero_min();
  test_eq_string_strf64_normal_max();
  test_eq_string_strf64_normal_zero();
  test_eq_string_strf64_normal_normal();
  test_eq_string_strf64_normal_min();
  test_eq_string_strf64_min_max();
  test_eq_string_strf64_min_zero();
  test_eq_string_strf64_min_normal();
  test_eq_string_strf64_min_min();
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
  test_eq_bool_f64_max_normal();
  test_eq_bool_f64_max_max();
  test_eq_bool_f64_max_min();
  test_eq_bool_f64_max_nan();
  test_eq_bool_f64_max_pinf();
  test_eq_bool_f64_max_zero();
  test_eq_bool_f64_max_ninf();
  test_eq_bool_f64_zero_normal();
  test_eq_bool_f64_zero_max();
  test_eq_bool_f64_zero_min();
  test_eq_bool_f64_zero_nan();
  test_eq_bool_f64_zero_pinf();
  test_eq_bool_f64_zero_zero();
  test_eq_bool_f64_zero_ninf();
  test_eq_bool_f64_normal_normal();
  test_eq_bool_f64_normal_max();
  test_eq_bool_f64_normal_min();
  test_eq_bool_f64_normal_nan();
  test_eq_bool_f64_normal_pinf();
  test_eq_bool_f64_normal_zero();
  test_eq_bool_f64_normal_ninf();
  test_eq_bool_f64_min_normal();
  test_eq_bool_f64_min_max();
  test_eq_bool_f64_min_min();
  test_eq_bool_f64_min_nan();
  test_eq_bool_f64_min_pinf();
  test_eq_bool_f64_min_zero();
  test_eq_bool_f64_min_ninf();
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
  test_eq_bool_stri32_max_max();
  test_eq_bool_stri32_max_zero();
  test_eq_bool_stri32_max_normal();
  test_eq_bool_stri32_max_min();
  test_eq_bool_stri32_zero_max();
  test_eq_bool_stri32_zero_zero();
  test_eq_bool_stri32_zero_normal();
  test_eq_bool_stri32_zero_min();
  test_eq_bool_stri32_normal_max();
  test_eq_bool_stri32_normal_zero();
  test_eq_bool_stri32_normal_normal();
  test_eq_bool_stri32_normal_min();
  test_eq_bool_stri32_min_max();
  test_eq_bool_stri32_min_zero();
  test_eq_bool_stri32_min_normal();
  test_eq_bool_stri32_min_min();
  test_eq_bool_strf64_max_max();
  test_eq_bool_strf64_max_zero();
  test_eq_bool_strf64_max_normal();
  test_eq_bool_strf64_max_min();
  test_eq_bool_strf64_zero_max();
  test_eq_bool_strf64_zero_zero();
  test_eq_bool_strf64_zero_normal();
  test_eq_bool_strf64_zero_min();
  test_eq_bool_strf64_normal_max();
  test_eq_bool_strf64_normal_zero();
  test_eq_bool_strf64_normal_normal();
  test_eq_bool_strf64_normal_min();
  test_eq_bool_strf64_min_max();
  test_eq_bool_strf64_min_zero();
  test_eq_bool_strf64_min_normal();
  test_eq_bool_strf64_min_min();
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
  test_eq_stri32_f64_max_normal();
  test_eq_stri32_f64_max_max();
  test_eq_stri32_f64_max_min();
  test_eq_stri32_f64_max_nan();
  test_eq_stri32_f64_max_pinf();
  test_eq_stri32_f64_max_zero();
  test_eq_stri32_f64_max_ninf();
  test_eq_stri32_f64_zero_normal();
  test_eq_stri32_f64_zero_max();
  test_eq_stri32_f64_zero_min();
  test_eq_stri32_f64_zero_nan();
  test_eq_stri32_f64_zero_pinf();
  test_eq_stri32_f64_zero_zero();
  test_eq_stri32_f64_zero_ninf();
  test_eq_stri32_f64_normal_normal();
  test_eq_stri32_f64_normal_max();
  test_eq_stri32_f64_normal_min();
  test_eq_stri32_f64_normal_nan();
  test_eq_stri32_f64_normal_pinf();
  test_eq_stri32_f64_normal_zero();
  test_eq_stri32_f64_normal_ninf();
  test_eq_stri32_f64_min_normal();
  test_eq_stri32_f64_min_max();
  test_eq_stri32_f64_min_min();
  test_eq_stri32_f64_min_nan();
  test_eq_stri32_f64_min_pinf();
  test_eq_stri32_f64_min_zero();
  test_eq_stri32_f64_min_ninf();
  test_eq_stri32_i32_max_max();
  test_eq_stri32_i32_max_zero();
  test_eq_stri32_i32_max_normal();
  test_eq_stri32_i32_max_min();
  test_eq_stri32_i32_zero_max();
  test_eq_stri32_i32_zero_zero();
  test_eq_stri32_i32_zero_normal();
  test_eq_stri32_i32_zero_min();
  test_eq_stri32_i32_normal_max();
  test_eq_stri32_i32_normal_zero();
  test_eq_stri32_i32_normal_normal();
  test_eq_stri32_i32_normal_min();
  test_eq_stri32_i32_min_max();
  test_eq_stri32_i32_min_zero();
  test_eq_stri32_i32_min_normal();
  test_eq_stri32_i32_min_min();
  test_eq_stri32_string_max_max();
  test_eq_stri32_string_max_zero();
  test_eq_stri32_string_max_normal();
  test_eq_stri32_string_max_min();
  test_eq_stri32_string_zero_max();
  test_eq_stri32_string_zero_zero();
  test_eq_stri32_string_zero_normal();
  test_eq_stri32_string_zero_min();
  test_eq_stri32_string_normal_max();
  test_eq_stri32_string_normal_zero();
  test_eq_stri32_string_normal_normal();
  test_eq_stri32_string_normal_min();
  test_eq_stri32_string_min_max();
  test_eq_stri32_string_min_zero();
  test_eq_stri32_string_min_normal();
  test_eq_stri32_string_min_min();
  test_eq_stri32_bool_max_max();
  test_eq_stri32_bool_max_zero();
  test_eq_stri32_bool_max_normal();
  test_eq_stri32_bool_max_min();
  test_eq_stri32_bool_zero_max();
  test_eq_stri32_bool_zero_zero();
  test_eq_stri32_bool_zero_normal();
  test_eq_stri32_bool_zero_min();
  test_eq_stri32_bool_normal_max();
  test_eq_stri32_bool_normal_zero();
  test_eq_stri32_bool_normal_normal();
  test_eq_stri32_bool_normal_min();
  test_eq_stri32_bool_min_max();
  test_eq_stri32_bool_min_zero();
  test_eq_stri32_bool_min_normal();
  test_eq_stri32_bool_min_min();
  test_eq_stri32_stri32_max_max();
  test_eq_stri32_stri32_max_zero();
  test_eq_stri32_stri32_max_normal();
  test_eq_stri32_stri32_max_min();
  test_eq_stri32_stri32_zero_max();
  test_eq_stri32_stri32_zero_zero();
  test_eq_stri32_stri32_zero_normal();
  test_eq_stri32_stri32_zero_min();
  test_eq_stri32_stri32_normal_max();
  test_eq_stri32_stri32_normal_zero();
  test_eq_stri32_stri32_normal_normal();
  test_eq_stri32_stri32_normal_min();
  test_eq_stri32_stri32_min_max();
  test_eq_stri32_stri32_min_zero();
  test_eq_stri32_stri32_min_normal();
  test_eq_stri32_stri32_min_min();
  test_eq_stri32_strf64_max_max();
  test_eq_stri32_strf64_max_zero();
  test_eq_stri32_strf64_max_normal();
  test_eq_stri32_strf64_max_min();
  test_eq_stri32_strf64_zero_max();
  test_eq_stri32_strf64_zero_zero();
  test_eq_stri32_strf64_zero_normal();
  test_eq_stri32_strf64_zero_min();
  test_eq_stri32_strf64_normal_max();
  test_eq_stri32_strf64_normal_zero();
  test_eq_stri32_strf64_normal_normal();
  test_eq_stri32_strf64_normal_min();
  test_eq_stri32_strf64_min_max();
  test_eq_stri32_strf64_min_zero();
  test_eq_stri32_strf64_min_normal();
  test_eq_stri32_strf64_min_min();
  test_eq_stri32_undefined_or_null_max_max();
  test_eq_stri32_undefined_or_null_max_zero();
  test_eq_stri32_undefined_or_null_max_normal();
  test_eq_stri32_undefined_or_null_max_min();
  test_eq_stri32_undefined_or_null_zero_max();
  test_eq_stri32_undefined_or_null_zero_zero();
  test_eq_stri32_undefined_or_null_zero_normal();
  test_eq_stri32_undefined_or_null_zero_min();
  test_eq_stri32_undefined_or_null_normal_max();
  test_eq_stri32_undefined_or_null_normal_zero();
  test_eq_stri32_undefined_or_null_normal_normal();
  test_eq_stri32_undefined_or_null_normal_min();
  test_eq_stri32_undefined_or_null_min_max();
  test_eq_stri32_undefined_or_null_min_zero();
  test_eq_stri32_undefined_or_null_min_normal();
  test_eq_stri32_undefined_or_null_min_min();
  test_eq_strf64_f64_max_normal();
  test_eq_strf64_f64_max_max();
  test_eq_strf64_f64_max_min();
  test_eq_strf64_f64_max_nan();
  test_eq_strf64_f64_max_pinf();
  test_eq_strf64_f64_max_zero();
  test_eq_strf64_f64_max_ninf();
  test_eq_strf64_f64_zero_normal();
  test_eq_strf64_f64_zero_max();
  test_eq_strf64_f64_zero_min();
  test_eq_strf64_f64_zero_nan();
  test_eq_strf64_f64_zero_pinf();
  test_eq_strf64_f64_zero_zero();
  test_eq_strf64_f64_zero_ninf();
  test_eq_strf64_f64_normal_normal();
  test_eq_strf64_f64_normal_max();
  test_eq_strf64_f64_normal_min();
  test_eq_strf64_f64_normal_nan();
  test_eq_strf64_f64_normal_pinf();
  test_eq_strf64_f64_normal_zero();
  test_eq_strf64_f64_normal_ninf();
  test_eq_strf64_f64_min_normal();
  test_eq_strf64_f64_min_max();
  test_eq_strf64_f64_min_min();
  test_eq_strf64_f64_min_nan();
  test_eq_strf64_f64_min_pinf();
  test_eq_strf64_f64_min_zero();
  test_eq_strf64_f64_min_ninf();
  test_eq_strf64_i32_max_max();
  test_eq_strf64_i32_max_zero();
  test_eq_strf64_i32_max_normal();
  test_eq_strf64_i32_max_min();
  test_eq_strf64_i32_zero_max();
  test_eq_strf64_i32_zero_zero();
  test_eq_strf64_i32_zero_normal();
  test_eq_strf64_i32_zero_min();
  test_eq_strf64_i32_normal_max();
  test_eq_strf64_i32_normal_zero();
  test_eq_strf64_i32_normal_normal();
  test_eq_strf64_i32_normal_min();
  test_eq_strf64_i32_min_max();
  test_eq_strf64_i32_min_zero();
  test_eq_strf64_i32_min_normal();
  test_eq_strf64_i32_min_min();
  test_eq_strf64_string_max_max();
  test_eq_strf64_string_max_zero();
  test_eq_strf64_string_max_normal();
  test_eq_strf64_string_max_min();
  test_eq_strf64_string_zero_max();
  test_eq_strf64_string_zero_zero();
  test_eq_strf64_string_zero_normal();
  test_eq_strf64_string_zero_min();
  test_eq_strf64_string_normal_max();
  test_eq_strf64_string_normal_zero();
  test_eq_strf64_string_normal_normal();
  test_eq_strf64_string_normal_min();
  test_eq_strf64_string_min_max();
  test_eq_strf64_string_min_zero();
  test_eq_strf64_string_min_normal();
  test_eq_strf64_string_min_min();
  test_eq_strf64_bool_max_max();
  test_eq_strf64_bool_max_zero();
  test_eq_strf64_bool_max_normal();
  test_eq_strf64_bool_max_min();
  test_eq_strf64_bool_zero_max();
  test_eq_strf64_bool_zero_zero();
  test_eq_strf64_bool_zero_normal();
  test_eq_strf64_bool_zero_min();
  test_eq_strf64_bool_normal_max();
  test_eq_strf64_bool_normal_zero();
  test_eq_strf64_bool_normal_normal();
  test_eq_strf64_bool_normal_min();
  test_eq_strf64_bool_min_max();
  test_eq_strf64_bool_min_zero();
  test_eq_strf64_bool_min_normal();
  test_eq_strf64_bool_min_min();
  test_eq_strf64_stri32_max_max();
  test_eq_strf64_stri32_max_zero();
  test_eq_strf64_stri32_max_normal();
  test_eq_strf64_stri32_max_min();
  test_eq_strf64_stri32_zero_max();
  test_eq_strf64_stri32_zero_zero();
  test_eq_strf64_stri32_zero_normal();
  test_eq_strf64_stri32_zero_min();
  test_eq_strf64_stri32_normal_max();
  test_eq_strf64_stri32_normal_zero();
  test_eq_strf64_stri32_normal_normal();
  test_eq_strf64_stri32_normal_min();
  test_eq_strf64_stri32_min_max();
  test_eq_strf64_stri32_min_zero();
  test_eq_strf64_stri32_min_normal();
  test_eq_strf64_stri32_min_min();
  test_eq_strf64_strf64_max_max();
  test_eq_strf64_strf64_max_zero();
  test_eq_strf64_strf64_max_normal();
  test_eq_strf64_strf64_max_min();
  test_eq_strf64_strf64_zero_max();
  test_eq_strf64_strf64_zero_zero();
  test_eq_strf64_strf64_zero_normal();
  test_eq_strf64_strf64_zero_min();
  test_eq_strf64_strf64_normal_max();
  test_eq_strf64_strf64_normal_zero();
  test_eq_strf64_strf64_normal_normal();
  test_eq_strf64_strf64_normal_min();
  test_eq_strf64_strf64_min_max();
  test_eq_strf64_strf64_min_zero();
  test_eq_strf64_strf64_min_normal();
  test_eq_strf64_strf64_min_min();
  test_eq_strf64_undefined_or_null_max_max();
  test_eq_strf64_undefined_or_null_max_zero();
  test_eq_strf64_undefined_or_null_max_normal();
  test_eq_strf64_undefined_or_null_max_min();
  test_eq_strf64_undefined_or_null_zero_max();
  test_eq_strf64_undefined_or_null_zero_zero();
  test_eq_strf64_undefined_or_null_zero_normal();
  test_eq_strf64_undefined_or_null_zero_min();
  test_eq_strf64_undefined_or_null_normal_max();
  test_eq_strf64_undefined_or_null_normal_zero();
  test_eq_strf64_undefined_or_null_normal_normal();
  test_eq_strf64_undefined_or_null_normal_min();
  test_eq_strf64_undefined_or_null_min_max();
  test_eq_strf64_undefined_or_null_min_zero();
  test_eq_strf64_undefined_or_null_min_normal();
  test_eq_strf64_undefined_or_null_min_min();
  test_eq_undefined_or_null_f64_max_normal();
  test_eq_undefined_or_null_f64_max_max();
  test_eq_undefined_or_null_f64_max_min();
  test_eq_undefined_or_null_f64_max_nan();
  test_eq_undefined_or_null_f64_max_pinf();
  test_eq_undefined_or_null_f64_max_zero();
  test_eq_undefined_or_null_f64_max_ninf();
  test_eq_undefined_or_null_f64_zero_normal();
  test_eq_undefined_or_null_f64_zero_max();
  test_eq_undefined_or_null_f64_zero_min();
  test_eq_undefined_or_null_f64_zero_nan();
  test_eq_undefined_or_null_f64_zero_pinf();
  test_eq_undefined_or_null_f64_zero_zero();
  test_eq_undefined_or_null_f64_zero_ninf();
  test_eq_undefined_or_null_f64_normal_normal();
  test_eq_undefined_or_null_f64_normal_max();
  test_eq_undefined_or_null_f64_normal_min();
  test_eq_undefined_or_null_f64_normal_nan();
  test_eq_undefined_or_null_f64_normal_pinf();
  test_eq_undefined_or_null_f64_normal_zero();
  test_eq_undefined_or_null_f64_normal_ninf();
  test_eq_undefined_or_null_f64_min_normal();
  test_eq_undefined_or_null_f64_min_max();
  test_eq_undefined_or_null_f64_min_min();
  test_eq_undefined_or_null_f64_min_nan();
  test_eq_undefined_or_null_f64_min_pinf();
  test_eq_undefined_or_null_f64_min_zero();
  test_eq_undefined_or_null_f64_min_ninf();
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
  test_eq_undefined_or_null_stri32_max_max();
  test_eq_undefined_or_null_stri32_max_zero();
  test_eq_undefined_or_null_stri32_max_normal();
  test_eq_undefined_or_null_stri32_max_min();
  test_eq_undefined_or_null_stri32_zero_max();
  test_eq_undefined_or_null_stri32_zero_zero();
  test_eq_undefined_or_null_stri32_zero_normal();
  test_eq_undefined_or_null_stri32_zero_min();
  test_eq_undefined_or_null_stri32_normal_max();
  test_eq_undefined_or_null_stri32_normal_zero();
  test_eq_undefined_or_null_stri32_normal_normal();
  test_eq_undefined_or_null_stri32_normal_min();
  test_eq_undefined_or_null_stri32_min_max();
  test_eq_undefined_or_null_stri32_min_zero();
  test_eq_undefined_or_null_stri32_min_normal();
  test_eq_undefined_or_null_stri32_min_min();
  test_eq_undefined_or_null_strf64_max_max();
  test_eq_undefined_or_null_strf64_max_zero();
  test_eq_undefined_or_null_strf64_max_normal();
  test_eq_undefined_or_null_strf64_max_min();
  test_eq_undefined_or_null_strf64_zero_max();
  test_eq_undefined_or_null_strf64_zero_zero();
  test_eq_undefined_or_null_strf64_zero_normal();
  test_eq_undefined_or_null_strf64_zero_min();
  test_eq_undefined_or_null_strf64_normal_max();
  test_eq_undefined_or_null_strf64_normal_zero();
  test_eq_undefined_or_null_strf64_normal_normal();
  test_eq_undefined_or_null_strf64_normal_min();
  test_eq_undefined_or_null_strf64_min_max();
  test_eq_undefined_or_null_strf64_min_zero();
  test_eq_undefined_or_null_strf64_min_normal();
  test_eq_undefined_or_null_strf64_min_min();
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