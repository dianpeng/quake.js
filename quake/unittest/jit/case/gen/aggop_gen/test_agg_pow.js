// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_agg_pow_f64_f64_normal_normal() {
  dump("testing: test_agg_pow_f64_f64_normal_normal");
  let a1 = 13.0;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_max() {
  dump("testing: test_agg_pow_f64_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_min() {
  dump("testing: test_agg_pow_f64_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_nan() {
  dump("testing: test_agg_pow_f64_f64_normal_nan");
  let a1 = NaN;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_pinf() {
  dump("testing: test_agg_pow_f64_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_zero() {
  dump("testing: test_agg_pow_f64_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_normal_ninf() {
  dump("testing: test_agg_pow_f64_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_normal() {
  dump("testing: test_agg_pow_f64_f64_max_normal");
  let a1 = 13.0;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_max() {
  dump("testing: test_agg_pow_f64_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_min() {
  dump("testing: test_agg_pow_f64_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_nan() {
  dump("testing: test_agg_pow_f64_f64_max_nan");
  let a1 = NaN;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_pinf() {
  dump("testing: test_agg_pow_f64_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_zero() {
  dump("testing: test_agg_pow_f64_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_max_ninf() {
  dump("testing: test_agg_pow_f64_f64_max_ninf");
  let a1 = Infinity;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_normal() {
  dump("testing: test_agg_pow_f64_f64_min_normal");
  let a1 = 13.0;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_max() {
  dump("testing: test_agg_pow_f64_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_min() {
  dump("testing: test_agg_pow_f64_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_nan() {
  dump("testing: test_agg_pow_f64_f64_min_nan");
  let a1 = NaN;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_pinf() {
  dump("testing: test_agg_pow_f64_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_zero() {
  dump("testing: test_agg_pow_f64_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_min_ninf() {
  dump("testing: test_agg_pow_f64_f64_min_ninf");
  let a1 = Infinity;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_normal() {
  dump("testing: test_agg_pow_f64_f64_nan_normal");
  let a1 = 13.0;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_max() {
  dump("testing: test_agg_pow_f64_f64_nan_max");
  let a1 = 1.79769313486e+308;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_min() {
  dump("testing: test_agg_pow_f64_f64_nan_min");
  let a1 = -1.79769313486e+308;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_nan() {
  dump("testing: test_agg_pow_f64_f64_nan_nan");
  let a1 = NaN;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_pinf() {
  dump("testing: test_agg_pow_f64_f64_nan_pinf");
  let a1 = -Infinity;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_zero() {
  dump("testing: test_agg_pow_f64_f64_nan_zero");
  let a1 = 2.22507385851e-308;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_nan_ninf() {
  dump("testing: test_agg_pow_f64_f64_nan_ninf");
  let a1 = Infinity;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_normal() {
  dump("testing: test_agg_pow_f64_f64_pinf_normal");
  let a1 = 13.0;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_max() {
  dump("testing: test_agg_pow_f64_f64_pinf_max");
  let a1 = 1.79769313486e+308;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_min() {
  dump("testing: test_agg_pow_f64_f64_pinf_min");
  let a1 = -1.79769313486e+308;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_nan() {
  dump("testing: test_agg_pow_f64_f64_pinf_nan");
  let a1 = NaN;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_pinf() {
  dump("testing: test_agg_pow_f64_f64_pinf_pinf");
  let a1 = -Infinity;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_zero() {
  dump("testing: test_agg_pow_f64_f64_pinf_zero");
  let a1 = 2.22507385851e-308;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_pinf_ninf() {
  dump("testing: test_agg_pow_f64_f64_pinf_ninf");
  let a1 = Infinity;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_normal() {
  dump("testing: test_agg_pow_f64_f64_zero_normal");
  let a1 = 13.0;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_max() {
  dump("testing: test_agg_pow_f64_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_min() {
  dump("testing: test_agg_pow_f64_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_nan() {
  dump("testing: test_agg_pow_f64_f64_zero_nan");
  let a1 = NaN;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_pinf() {
  dump("testing: test_agg_pow_f64_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_zero() {
  dump("testing: test_agg_pow_f64_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_zero_ninf() {
  dump("testing: test_agg_pow_f64_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_normal() {
  dump("testing: test_agg_pow_f64_f64_ninf_normal");
  let a1 = 13.0;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_max() {
  dump("testing: test_agg_pow_f64_f64_ninf_max");
  let a1 = 1.79769313486e+308;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_min() {
  dump("testing: test_agg_pow_f64_f64_ninf_min");
  let a1 = -1.79769313486e+308;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_nan() {
  dump("testing: test_agg_pow_f64_f64_ninf_nan");
  let a1 = NaN;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_pinf() {
  dump("testing: test_agg_pow_f64_f64_ninf_pinf");
  let a1 = -Infinity;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_zero() {
  dump("testing: test_agg_pow_f64_f64_ninf_zero");
  let a1 = 2.22507385851e-308;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_f64_ninf_ninf() {
  dump("testing: test_agg_pow_f64_f64_ninf_ninf");
  let a1 = Infinity;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_normal_max() {
  dump("testing: test_agg_pow_f64_i32_normal_max");
  let a1 = 2147483647;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_normal_zero() {
  dump("testing: test_agg_pow_f64_i32_normal_zero");
  let a1 = 0;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_normal_normal() {
  dump("testing: test_agg_pow_f64_i32_normal_normal");
  let a1 = 14;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_normal_min() {
  dump("testing: test_agg_pow_f64_i32_normal_min");
  let a1 = -2147483648;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_max_max() {
  dump("testing: test_agg_pow_f64_i32_max_max");
  let a1 = 2147483647;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_max_zero() {
  dump("testing: test_agg_pow_f64_i32_max_zero");
  let a1 = 0;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_max_normal() {
  dump("testing: test_agg_pow_f64_i32_max_normal");
  let a1 = 14;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_max_min() {
  dump("testing: test_agg_pow_f64_i32_max_min");
  let a1 = -2147483648;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_min_max() {
  dump("testing: test_agg_pow_f64_i32_min_max");
  let a1 = 2147483647;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_min_zero() {
  dump("testing: test_agg_pow_f64_i32_min_zero");
  let a1 = 0;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_min_normal() {
  dump("testing: test_agg_pow_f64_i32_min_normal");
  let a1 = 14;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_min_min() {
  dump("testing: test_agg_pow_f64_i32_min_min");
  let a1 = -2147483648;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_nan_max() {
  dump("testing: test_agg_pow_f64_i32_nan_max");
  let a1 = 2147483647;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_nan_zero() {
  dump("testing: test_agg_pow_f64_i32_nan_zero");
  let a1 = 0;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_nan_normal() {
  dump("testing: test_agg_pow_f64_i32_nan_normal");
  let a1 = 14;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_nan_min() {
  dump("testing: test_agg_pow_f64_i32_nan_min");
  let a1 = -2147483648;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_pinf_max() {
  dump("testing: test_agg_pow_f64_i32_pinf_max");
  let a1 = 2147483647;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_pinf_zero() {
  dump("testing: test_agg_pow_f64_i32_pinf_zero");
  let a1 = 0;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_pinf_normal() {
  dump("testing: test_agg_pow_f64_i32_pinf_normal");
  let a1 = 14;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_pinf_min() {
  dump("testing: test_agg_pow_f64_i32_pinf_min");
  let a1 = -2147483648;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_zero_max() {
  dump("testing: test_agg_pow_f64_i32_zero_max");
  let a1 = 2147483647;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_zero_zero() {
  dump("testing: test_agg_pow_f64_i32_zero_zero");
  let a1 = 0;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_zero_normal() {
  dump("testing: test_agg_pow_f64_i32_zero_normal");
  let a1 = 14;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_zero_min() {
  dump("testing: test_agg_pow_f64_i32_zero_min");
  let a1 = -2147483648;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_ninf_max() {
  dump("testing: test_agg_pow_f64_i32_ninf_max");
  let a1 = 2147483647;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_ninf_zero() {
  dump("testing: test_agg_pow_f64_i32_ninf_zero");
  let a1 = 0;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_ninf_normal() {
  dump("testing: test_agg_pow_f64_i32_ninf_normal");
  let a1 = 14;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_i32_ninf_min() {
  dump("testing: test_agg_pow_f64_i32_ninf_min");
  let a1 = -2147483648;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_normal_max() {
  dump("testing: test_agg_pow_f64_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_normal_zero() {
  dump("testing: test_agg_pow_f64_stri32_normal_zero");
  let a1 = '1';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_normal_normal() {
  dump("testing: test_agg_pow_f64_stri32_normal_normal");
  let a1 = '1000';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_normal_min() {
  dump("testing: test_agg_pow_f64_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_max_max() {
  dump("testing: test_agg_pow_f64_stri32_max_max");
  let a1 = '2147483647';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_max_zero() {
  dump("testing: test_agg_pow_f64_stri32_max_zero");
  let a1 = '1';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_max_normal() {
  dump("testing: test_agg_pow_f64_stri32_max_normal");
  let a1 = '1000';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_max_min() {
  dump("testing: test_agg_pow_f64_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_min_max() {
  dump("testing: test_agg_pow_f64_stri32_min_max");
  let a1 = '2147483647';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_min_zero() {
  dump("testing: test_agg_pow_f64_stri32_min_zero");
  let a1 = '1';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_min_normal() {
  dump("testing: test_agg_pow_f64_stri32_min_normal");
  let a1 = '1000';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_min_min() {
  dump("testing: test_agg_pow_f64_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_nan_max() {
  dump("testing: test_agg_pow_f64_stri32_nan_max");
  let a1 = '2147483647';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_nan_zero() {
  dump("testing: test_agg_pow_f64_stri32_nan_zero");
  let a1 = '1';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_nan_normal() {
  dump("testing: test_agg_pow_f64_stri32_nan_normal");
  let a1 = '1000';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_nan_min() {
  dump("testing: test_agg_pow_f64_stri32_nan_min");
  let a1 = '-2147483648';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_pinf_max() {
  dump("testing: test_agg_pow_f64_stri32_pinf_max");
  let a1 = '2147483647';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_pinf_zero() {
  dump("testing: test_agg_pow_f64_stri32_pinf_zero");
  let a1 = '1';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_pinf_normal() {
  dump("testing: test_agg_pow_f64_stri32_pinf_normal");
  let a1 = '1000';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_pinf_min() {
  dump("testing: test_agg_pow_f64_stri32_pinf_min");
  let a1 = '-2147483648';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_zero_max() {
  dump("testing: test_agg_pow_f64_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_zero_zero() {
  dump("testing: test_agg_pow_f64_stri32_zero_zero");
  let a1 = '1';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_zero_normal() {
  dump("testing: test_agg_pow_f64_stri32_zero_normal");
  let a1 = '1000';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_zero_min() {
  dump("testing: test_agg_pow_f64_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_ninf_max() {
  dump("testing: test_agg_pow_f64_stri32_ninf_max");
  let a1 = '2147483647';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_ninf_zero() {
  dump("testing: test_agg_pow_f64_stri32_ninf_zero");
  let a1 = '1';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_ninf_normal() {
  dump("testing: test_agg_pow_f64_stri32_ninf_normal");
  let a1 = '1000';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_stri32_ninf_min() {
  dump("testing: test_agg_pow_f64_stri32_ninf_min");
  let a1 = '-2147483648';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_normal_max() {
  dump("testing: test_agg_pow_f64_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_normal_zero() {
  dump("testing: test_agg_pow_f64_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_normal_normal() {
  dump("testing: test_agg_pow_f64_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_normal_min() {
  dump("testing: test_agg_pow_f64_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_max_max() {
  dump("testing: test_agg_pow_f64_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_max_zero() {
  dump("testing: test_agg_pow_f64_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_max_normal() {
  dump("testing: test_agg_pow_f64_strf64_max_normal");
  let a1 = '13.2';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_max_min() {
  dump("testing: test_agg_pow_f64_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_min_max() {
  dump("testing: test_agg_pow_f64_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_min_zero() {
  dump("testing: test_agg_pow_f64_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_min_normal() {
  dump("testing: test_agg_pow_f64_strf64_min_normal");
  let a1 = '13.2';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_min_min() {
  dump("testing: test_agg_pow_f64_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_nan_max() {
  dump("testing: test_agg_pow_f64_strf64_nan_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_nan_zero() {
  dump("testing: test_agg_pow_f64_strf64_nan_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_nan_normal() {
  dump("testing: test_agg_pow_f64_strf64_nan_normal");
  let a1 = '13.2';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_nan_min() {
  dump("testing: test_agg_pow_f64_strf64_nan_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_pinf_max() {
  dump("testing: test_agg_pow_f64_strf64_pinf_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_pinf_zero() {
  dump("testing: test_agg_pow_f64_strf64_pinf_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_pinf_normal() {
  dump("testing: test_agg_pow_f64_strf64_pinf_normal");
  let a1 = '13.2';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_pinf_min() {
  dump("testing: test_agg_pow_f64_strf64_pinf_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_zero_max() {
  dump("testing: test_agg_pow_f64_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_zero_zero() {
  dump("testing: test_agg_pow_f64_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_zero_normal() {
  dump("testing: test_agg_pow_f64_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_zero_min() {
  dump("testing: test_agg_pow_f64_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_ninf_max() {
  dump("testing: test_agg_pow_f64_strf64_ninf_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_ninf_zero() {
  dump("testing: test_agg_pow_f64_strf64_ninf_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_ninf_normal() {
  dump("testing: test_agg_pow_f64_strf64_ninf_normal");
  let a1 = '13.2';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_strf64_ninf_min() {
  dump("testing: test_agg_pow_f64_strf64_ninf_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_normal_max() {
  dump("testing: test_agg_pow_f64_bool_normal_max");
  let a1 = true;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_normal_zero() {
  dump("testing: test_agg_pow_f64_bool_normal_zero");
  let a1 = false;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_normal_normal() {
  dump("testing: test_agg_pow_f64_bool_normal_normal");
  let a1 = true;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_normal_min() {
  dump("testing: test_agg_pow_f64_bool_normal_min");
  let a1 = false;
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_max_max() {
  dump("testing: test_agg_pow_f64_bool_max_max");
  let a1 = true;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_max_zero() {
  dump("testing: test_agg_pow_f64_bool_max_zero");
  let a1 = false;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_max_normal() {
  dump("testing: test_agg_pow_f64_bool_max_normal");
  let a1 = true;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_max_min() {
  dump("testing: test_agg_pow_f64_bool_max_min");
  let a1 = false;
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_min_max() {
  dump("testing: test_agg_pow_f64_bool_min_max");
  let a1 = true;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_min_zero() {
  dump("testing: test_agg_pow_f64_bool_min_zero");
  let a1 = false;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_min_normal() {
  dump("testing: test_agg_pow_f64_bool_min_normal");
  let a1 = true;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_min_min() {
  dump("testing: test_agg_pow_f64_bool_min_min");
  let a1 = false;
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_nan_max() {
  dump("testing: test_agg_pow_f64_bool_nan_max");
  let a1 = true;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_nan_zero() {
  dump("testing: test_agg_pow_f64_bool_nan_zero");
  let a1 = false;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_nan_normal() {
  dump("testing: test_agg_pow_f64_bool_nan_normal");
  let a1 = true;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_nan_min() {
  dump("testing: test_agg_pow_f64_bool_nan_min");
  let a1 = false;
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_pinf_max() {
  dump("testing: test_agg_pow_f64_bool_pinf_max");
  let a1 = true;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_pinf_zero() {
  dump("testing: test_agg_pow_f64_bool_pinf_zero");
  let a1 = false;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_pinf_normal() {
  dump("testing: test_agg_pow_f64_bool_pinf_normal");
  let a1 = true;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_pinf_min() {
  dump("testing: test_agg_pow_f64_bool_pinf_min");
  let a1 = false;
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_zero_max() {
  dump("testing: test_agg_pow_f64_bool_zero_max");
  let a1 = true;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_zero_zero() {
  dump("testing: test_agg_pow_f64_bool_zero_zero");
  let a1 = false;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_zero_normal() {
  dump("testing: test_agg_pow_f64_bool_zero_normal");
  let a1 = true;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_zero_min() {
  dump("testing: test_agg_pow_f64_bool_zero_min");
  let a1 = false;
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_ninf_max() {
  dump("testing: test_agg_pow_f64_bool_ninf_max");
  let a1 = true;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_ninf_zero() {
  dump("testing: test_agg_pow_f64_bool_ninf_zero");
  let a1 = false;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_ninf_normal() {
  dump("testing: test_agg_pow_f64_bool_ninf_normal");
  let a1 = true;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_bool_ninf_min() {
  dump("testing: test_agg_pow_f64_bool_ninf_min");
  let a1 = false;
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_normal_max() {
  dump("testing: test_agg_pow_f64_object_normal_max");
  let a1 = {'a':1};
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_normal_zero() {
  dump("testing: test_agg_pow_f64_object_normal_zero");
  let a1 = {};
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_normal_normal() {
  dump("testing: test_agg_pow_f64_object_normal_normal");
  let a1 = {'a':100};
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_normal_min() {
  dump("testing: test_agg_pow_f64_object_normal_min");
  let a1 = {};
  let test_value = 13.0;
  let original = 13.0;

  let expect = 13.0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_max_max() {
  dump("testing: test_agg_pow_f64_object_max_max");
  let a1 = {'a':1};
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_max_zero() {
  dump("testing: test_agg_pow_f64_object_max_zero");
  let a1 = {};
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_max_normal() {
  dump("testing: test_agg_pow_f64_object_max_normal");
  let a1 = {'a':100};
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_max_min() {
  dump("testing: test_agg_pow_f64_object_max_min");
  let a1 = {};
  let test_value = 1.79769313486e+308;
  let original = 1.79769313486e+308;

  let expect = 1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_min_max() {
  dump("testing: test_agg_pow_f64_object_min_max");
  let a1 = {'a':1};
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_min_zero() {
  dump("testing: test_agg_pow_f64_object_min_zero");
  let a1 = {};
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_min_normal() {
  dump("testing: test_agg_pow_f64_object_min_normal");
  let a1 = {'a':100};
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_min_min() {
  dump("testing: test_agg_pow_f64_object_min_min");
  let a1 = {};
  let test_value = -1.79769313486e+308;
  let original = -1.79769313486e+308;

  let expect = -1.79769313486e+308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_nan_max() {
  dump("testing: test_agg_pow_f64_object_nan_max");
  let a1 = {'a':1};
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_nan_zero() {
  dump("testing: test_agg_pow_f64_object_nan_zero");
  let a1 = {};
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_nan_normal() {
  dump("testing: test_agg_pow_f64_object_nan_normal");
  let a1 = {'a':100};
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_nan_min() {
  dump("testing: test_agg_pow_f64_object_nan_min");
  let a1 = {};
  let test_value = NaN;
  let original = NaN;

  let expect = NaN;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_pinf_max() {
  dump("testing: test_agg_pow_f64_object_pinf_max");
  let a1 = {'a':1};
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_pinf_zero() {
  dump("testing: test_agg_pow_f64_object_pinf_zero");
  let a1 = {};
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_pinf_normal() {
  dump("testing: test_agg_pow_f64_object_pinf_normal");
  let a1 = {'a':100};
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_pinf_min() {
  dump("testing: test_agg_pow_f64_object_pinf_min");
  let a1 = {};
  let test_value = -Infinity;
  let original = -Infinity;

  let expect = -Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_zero_max() {
  dump("testing: test_agg_pow_f64_object_zero_max");
  let a1 = {'a':1};
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_zero_zero() {
  dump("testing: test_agg_pow_f64_object_zero_zero");
  let a1 = {};
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_zero_normal() {
  dump("testing: test_agg_pow_f64_object_zero_normal");
  let a1 = {'a':100};
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_zero_min() {
  dump("testing: test_agg_pow_f64_object_zero_min");
  let a1 = {};
  let test_value = 2.22507385851e-308;
  let original = 2.22507385851e-308;

  let expect = 2.22507385851e-308;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_ninf_max() {
  dump("testing: test_agg_pow_f64_object_ninf_max");
  let a1 = {'a':1};
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_ninf_zero() {
  dump("testing: test_agg_pow_f64_object_ninf_zero");
  let a1 = {};
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_ninf_normal() {
  dump("testing: test_agg_pow_f64_object_ninf_normal");
  let a1 = {'a':100};
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_f64_object_ninf_min() {
  dump("testing: test_agg_pow_f64_object_ninf_min");
  let a1 = {};
  let test_value = Infinity;
  let original = Infinity;

  let expect = Infinity;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_normal() {
  dump("testing: test_agg_pow_i32_f64_max_normal");
  let a1 = 13.0;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_max() {
  dump("testing: test_agg_pow_i32_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_min() {
  dump("testing: test_agg_pow_i32_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_nan() {
  dump("testing: test_agg_pow_i32_f64_max_nan");
  let a1 = NaN;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_pinf() {
  dump("testing: test_agg_pow_i32_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_zero() {
  dump("testing: test_agg_pow_i32_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_max_ninf() {
  dump("testing: test_agg_pow_i32_f64_max_ninf");
  let a1 = Infinity;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_normal() {
  dump("testing: test_agg_pow_i32_f64_zero_normal");
  let a1 = 13.0;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_max() {
  dump("testing: test_agg_pow_i32_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_min() {
  dump("testing: test_agg_pow_i32_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_nan() {
  dump("testing: test_agg_pow_i32_f64_zero_nan");
  let a1 = NaN;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_pinf() {
  dump("testing: test_agg_pow_i32_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_zero() {
  dump("testing: test_agg_pow_i32_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_zero_ninf() {
  dump("testing: test_agg_pow_i32_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_normal() {
  dump("testing: test_agg_pow_i32_f64_normal_normal");
  let a1 = 13.0;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_max() {
  dump("testing: test_agg_pow_i32_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_min() {
  dump("testing: test_agg_pow_i32_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_nan() {
  dump("testing: test_agg_pow_i32_f64_normal_nan");
  let a1 = NaN;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_pinf() {
  dump("testing: test_agg_pow_i32_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_zero() {
  dump("testing: test_agg_pow_i32_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_normal_ninf() {
  dump("testing: test_agg_pow_i32_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_normal() {
  dump("testing: test_agg_pow_i32_f64_min_normal");
  let a1 = 13.0;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_max() {
  dump("testing: test_agg_pow_i32_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_min() {
  dump("testing: test_agg_pow_i32_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_nan() {
  dump("testing: test_agg_pow_i32_f64_min_nan");
  let a1 = NaN;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_pinf() {
  dump("testing: test_agg_pow_i32_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_zero() {
  dump("testing: test_agg_pow_i32_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_f64_min_ninf() {
  dump("testing: test_agg_pow_i32_f64_min_ninf");
  let a1 = Infinity;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_max_max() {
  dump("testing: test_agg_pow_i32_i32_max_max");
  let a1 = 2147483647;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_max_zero() {
  dump("testing: test_agg_pow_i32_i32_max_zero");
  let a1 = 0;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_max_normal() {
  dump("testing: test_agg_pow_i32_i32_max_normal");
  let a1 = 14;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_max_min() {
  dump("testing: test_agg_pow_i32_i32_max_min");
  let a1 = -2147483648;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_zero_max() {
  dump("testing: test_agg_pow_i32_i32_zero_max");
  let a1 = 2147483647;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_zero_zero() {
  dump("testing: test_agg_pow_i32_i32_zero_zero");
  let a1 = 0;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_zero_normal() {
  dump("testing: test_agg_pow_i32_i32_zero_normal");
  let a1 = 14;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_zero_min() {
  dump("testing: test_agg_pow_i32_i32_zero_min");
  let a1 = -2147483648;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_normal_max() {
  dump("testing: test_agg_pow_i32_i32_normal_max");
  let a1 = 2147483647;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_normal_zero() {
  dump("testing: test_agg_pow_i32_i32_normal_zero");
  let a1 = 0;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_normal_normal() {
  dump("testing: test_agg_pow_i32_i32_normal_normal");
  let a1 = 14;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_normal_min() {
  dump("testing: test_agg_pow_i32_i32_normal_min");
  let a1 = -2147483648;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_min_max() {
  dump("testing: test_agg_pow_i32_i32_min_max");
  let a1 = 2147483647;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_min_zero() {
  dump("testing: test_agg_pow_i32_i32_min_zero");
  let a1 = 0;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_min_normal() {
  dump("testing: test_agg_pow_i32_i32_min_normal");
  let a1 = 14;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_i32_min_min() {
  dump("testing: test_agg_pow_i32_i32_min_min");
  let a1 = -2147483648;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_max_max() {
  dump("testing: test_agg_pow_i32_stri32_max_max");
  let a1 = '2147483647';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_max_zero() {
  dump("testing: test_agg_pow_i32_stri32_max_zero");
  let a1 = '1';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_max_normal() {
  dump("testing: test_agg_pow_i32_stri32_max_normal");
  let a1 = '1000';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_max_min() {
  dump("testing: test_agg_pow_i32_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_zero_max() {
  dump("testing: test_agg_pow_i32_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_zero_zero() {
  dump("testing: test_agg_pow_i32_stri32_zero_zero");
  let a1 = '1';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_zero_normal() {
  dump("testing: test_agg_pow_i32_stri32_zero_normal");
  let a1 = '1000';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_zero_min() {
  dump("testing: test_agg_pow_i32_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_normal_max() {
  dump("testing: test_agg_pow_i32_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_normal_zero() {
  dump("testing: test_agg_pow_i32_stri32_normal_zero");
  let a1 = '1';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_normal_normal() {
  dump("testing: test_agg_pow_i32_stri32_normal_normal");
  let a1 = '1000';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_normal_min() {
  dump("testing: test_agg_pow_i32_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_min_max() {
  dump("testing: test_agg_pow_i32_stri32_min_max");
  let a1 = '2147483647';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_min_zero() {
  dump("testing: test_agg_pow_i32_stri32_min_zero");
  let a1 = '1';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_min_normal() {
  dump("testing: test_agg_pow_i32_stri32_min_normal");
  let a1 = '1000';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_stri32_min_min() {
  dump("testing: test_agg_pow_i32_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_max_max() {
  dump("testing: test_agg_pow_i32_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_max_zero() {
  dump("testing: test_agg_pow_i32_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_max_normal() {
  dump("testing: test_agg_pow_i32_strf64_max_normal");
  let a1 = '13.2';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_max_min() {
  dump("testing: test_agg_pow_i32_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_zero_max() {
  dump("testing: test_agg_pow_i32_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_zero_zero() {
  dump("testing: test_agg_pow_i32_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_zero_normal() {
  dump("testing: test_agg_pow_i32_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_zero_min() {
  dump("testing: test_agg_pow_i32_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_normal_max() {
  dump("testing: test_agg_pow_i32_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_normal_zero() {
  dump("testing: test_agg_pow_i32_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_normal_normal() {
  dump("testing: test_agg_pow_i32_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_normal_min() {
  dump("testing: test_agg_pow_i32_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_min_max() {
  dump("testing: test_agg_pow_i32_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_min_zero() {
  dump("testing: test_agg_pow_i32_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_min_normal() {
  dump("testing: test_agg_pow_i32_strf64_min_normal");
  let a1 = '13.2';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_strf64_min_min() {
  dump("testing: test_agg_pow_i32_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_max_max() {
  dump("testing: test_agg_pow_i32_bool_max_max");
  let a1 = true;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_max_zero() {
  dump("testing: test_agg_pow_i32_bool_max_zero");
  let a1 = false;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_max_normal() {
  dump("testing: test_agg_pow_i32_bool_max_normal");
  let a1 = true;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_max_min() {
  dump("testing: test_agg_pow_i32_bool_max_min");
  let a1 = false;
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_zero_max() {
  dump("testing: test_agg_pow_i32_bool_zero_max");
  let a1 = true;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_zero_zero() {
  dump("testing: test_agg_pow_i32_bool_zero_zero");
  let a1 = false;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_zero_normal() {
  dump("testing: test_agg_pow_i32_bool_zero_normal");
  let a1 = true;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_zero_min() {
  dump("testing: test_agg_pow_i32_bool_zero_min");
  let a1 = false;
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_normal_max() {
  dump("testing: test_agg_pow_i32_bool_normal_max");
  let a1 = true;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_normal_zero() {
  dump("testing: test_agg_pow_i32_bool_normal_zero");
  let a1 = false;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_normal_normal() {
  dump("testing: test_agg_pow_i32_bool_normal_normal");
  let a1 = true;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_normal_min() {
  dump("testing: test_agg_pow_i32_bool_normal_min");
  let a1 = false;
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_min_max() {
  dump("testing: test_agg_pow_i32_bool_min_max");
  let a1 = true;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_min_zero() {
  dump("testing: test_agg_pow_i32_bool_min_zero");
  let a1 = false;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_min_normal() {
  dump("testing: test_agg_pow_i32_bool_min_normal");
  let a1 = true;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_bool_min_min() {
  dump("testing: test_agg_pow_i32_bool_min_min");
  let a1 = false;
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_max_max() {
  dump("testing: test_agg_pow_i32_object_max_max");
  let a1 = {'a':1};
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_max_zero() {
  dump("testing: test_agg_pow_i32_object_max_zero");
  let a1 = {};
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_max_normal() {
  dump("testing: test_agg_pow_i32_object_max_normal");
  let a1 = {'a':100};
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_max_min() {
  dump("testing: test_agg_pow_i32_object_max_min");
  let a1 = {};
  let test_value = 2147483647;
  let original = 2147483647;

  let expect = 2147483647;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_zero_max() {
  dump("testing: test_agg_pow_i32_object_zero_max");
  let a1 = {'a':1};
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_zero_zero() {
  dump("testing: test_agg_pow_i32_object_zero_zero");
  let a1 = {};
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_zero_normal() {
  dump("testing: test_agg_pow_i32_object_zero_normal");
  let a1 = {'a':100};
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_zero_min() {
  dump("testing: test_agg_pow_i32_object_zero_min");
  let a1 = {};
  let test_value = 0;
  let original = 0;

  let expect = 0;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_normal_max() {
  dump("testing: test_agg_pow_i32_object_normal_max");
  let a1 = {'a':1};
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_normal_zero() {
  dump("testing: test_agg_pow_i32_object_normal_zero");
  let a1 = {};
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_normal_normal() {
  dump("testing: test_agg_pow_i32_object_normal_normal");
  let a1 = {'a':100};
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_normal_min() {
  dump("testing: test_agg_pow_i32_object_normal_min");
  let a1 = {};
  let test_value = 14;
  let original = 14;

  let expect = 14;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_min_max() {
  dump("testing: test_agg_pow_i32_object_min_max");
  let a1 = {'a':1};
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_min_zero() {
  dump("testing: test_agg_pow_i32_object_min_zero");
  let a1 = {};
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_min_normal() {
  dump("testing: test_agg_pow_i32_object_min_normal");
  let a1 = {'a':100};
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_i32_object_min_min() {
  dump("testing: test_agg_pow_i32_object_min_min");
  let a1 = {};
  let test_value = -2147483648;
  let original = -2147483648;

  let expect = -2147483648;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_normal() {
  dump("testing: test_agg_pow_stri32_f64_max_normal");
  let a1 = 13.0;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_max() {
  dump("testing: test_agg_pow_stri32_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_min() {
  dump("testing: test_agg_pow_stri32_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_nan() {
  dump("testing: test_agg_pow_stri32_f64_max_nan");
  let a1 = NaN;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_pinf() {
  dump("testing: test_agg_pow_stri32_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_zero() {
  dump("testing: test_agg_pow_stri32_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_max_ninf() {
  dump("testing: test_agg_pow_stri32_f64_max_ninf");
  let a1 = Infinity;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_normal() {
  dump("testing: test_agg_pow_stri32_f64_zero_normal");
  let a1 = 13.0;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_max() {
  dump("testing: test_agg_pow_stri32_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_min() {
  dump("testing: test_agg_pow_stri32_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_nan() {
  dump("testing: test_agg_pow_stri32_f64_zero_nan");
  let a1 = NaN;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_pinf() {
  dump("testing: test_agg_pow_stri32_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_zero() {
  dump("testing: test_agg_pow_stri32_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_zero_ninf() {
  dump("testing: test_agg_pow_stri32_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_normal() {
  dump("testing: test_agg_pow_stri32_f64_normal_normal");
  let a1 = 13.0;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_max() {
  dump("testing: test_agg_pow_stri32_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_min() {
  dump("testing: test_agg_pow_stri32_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_nan() {
  dump("testing: test_agg_pow_stri32_f64_normal_nan");
  let a1 = NaN;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_pinf() {
  dump("testing: test_agg_pow_stri32_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_zero() {
  dump("testing: test_agg_pow_stri32_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_normal_ninf() {
  dump("testing: test_agg_pow_stri32_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_normal() {
  dump("testing: test_agg_pow_stri32_f64_min_normal");
  let a1 = 13.0;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_max() {
  dump("testing: test_agg_pow_stri32_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_min() {
  dump("testing: test_agg_pow_stri32_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_nan() {
  dump("testing: test_agg_pow_stri32_f64_min_nan");
  let a1 = NaN;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_pinf() {
  dump("testing: test_agg_pow_stri32_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_zero() {
  dump("testing: test_agg_pow_stri32_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_f64_min_ninf() {
  dump("testing: test_agg_pow_stri32_f64_min_ninf");
  let a1 = Infinity;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_max_max() {
  dump("testing: test_agg_pow_stri32_i32_max_max");
  let a1 = 2147483647;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_max_zero() {
  dump("testing: test_agg_pow_stri32_i32_max_zero");
  let a1 = 0;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_max_normal() {
  dump("testing: test_agg_pow_stri32_i32_max_normal");
  let a1 = 14;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_max_min() {
  dump("testing: test_agg_pow_stri32_i32_max_min");
  let a1 = -2147483648;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_zero_max() {
  dump("testing: test_agg_pow_stri32_i32_zero_max");
  let a1 = 2147483647;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_zero_zero() {
  dump("testing: test_agg_pow_stri32_i32_zero_zero");
  let a1 = 0;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_zero_normal() {
  dump("testing: test_agg_pow_stri32_i32_zero_normal");
  let a1 = 14;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_zero_min() {
  dump("testing: test_agg_pow_stri32_i32_zero_min");
  let a1 = -2147483648;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_normal_max() {
  dump("testing: test_agg_pow_stri32_i32_normal_max");
  let a1 = 2147483647;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_normal_zero() {
  dump("testing: test_agg_pow_stri32_i32_normal_zero");
  let a1 = 0;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_normal_normal() {
  dump("testing: test_agg_pow_stri32_i32_normal_normal");
  let a1 = 14;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_normal_min() {
  dump("testing: test_agg_pow_stri32_i32_normal_min");
  let a1 = -2147483648;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_min_max() {
  dump("testing: test_agg_pow_stri32_i32_min_max");
  let a1 = 2147483647;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_min_zero() {
  dump("testing: test_agg_pow_stri32_i32_min_zero");
  let a1 = 0;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_min_normal() {
  dump("testing: test_agg_pow_stri32_i32_min_normal");
  let a1 = 14;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_i32_min_min() {
  dump("testing: test_agg_pow_stri32_i32_min_min");
  let a1 = -2147483648;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_max_max() {
  dump("testing: test_agg_pow_stri32_stri32_max_max");
  let a1 = '2147483647';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_max_zero() {
  dump("testing: test_agg_pow_stri32_stri32_max_zero");
  let a1 = '1';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_max_normal() {
  dump("testing: test_agg_pow_stri32_stri32_max_normal");
  let a1 = '1000';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_max_min() {
  dump("testing: test_agg_pow_stri32_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_zero_max() {
  dump("testing: test_agg_pow_stri32_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_zero_zero() {
  dump("testing: test_agg_pow_stri32_stri32_zero_zero");
  let a1 = '1';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_zero_normal() {
  dump("testing: test_agg_pow_stri32_stri32_zero_normal");
  let a1 = '1000';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_zero_min() {
  dump("testing: test_agg_pow_stri32_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_normal_max() {
  dump("testing: test_agg_pow_stri32_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_normal_zero() {
  dump("testing: test_agg_pow_stri32_stri32_normal_zero");
  let a1 = '1';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_normal_normal() {
  dump("testing: test_agg_pow_stri32_stri32_normal_normal");
  let a1 = '1000';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_normal_min() {
  dump("testing: test_agg_pow_stri32_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_min_max() {
  dump("testing: test_agg_pow_stri32_stri32_min_max");
  let a1 = '2147483647';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_min_zero() {
  dump("testing: test_agg_pow_stri32_stri32_min_zero");
  let a1 = '1';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_min_normal() {
  dump("testing: test_agg_pow_stri32_stri32_min_normal");
  let a1 = '1000';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_stri32_min_min() {
  dump("testing: test_agg_pow_stri32_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_max_max() {
  dump("testing: test_agg_pow_stri32_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_max_zero() {
  dump("testing: test_agg_pow_stri32_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_max_normal() {
  dump("testing: test_agg_pow_stri32_strf64_max_normal");
  let a1 = '13.2';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_max_min() {
  dump("testing: test_agg_pow_stri32_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_zero_max() {
  dump("testing: test_agg_pow_stri32_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_zero_zero() {
  dump("testing: test_agg_pow_stri32_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_zero_normal() {
  dump("testing: test_agg_pow_stri32_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_zero_min() {
  dump("testing: test_agg_pow_stri32_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_normal_max() {
  dump("testing: test_agg_pow_stri32_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_normal_zero() {
  dump("testing: test_agg_pow_stri32_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_normal_normal() {
  dump("testing: test_agg_pow_stri32_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_normal_min() {
  dump("testing: test_agg_pow_stri32_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_min_max() {
  dump("testing: test_agg_pow_stri32_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_min_zero() {
  dump("testing: test_agg_pow_stri32_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_min_normal() {
  dump("testing: test_agg_pow_stri32_strf64_min_normal");
  let a1 = '13.2';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_strf64_min_min() {
  dump("testing: test_agg_pow_stri32_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_max_max() {
  dump("testing: test_agg_pow_stri32_bool_max_max");
  let a1 = true;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_max_zero() {
  dump("testing: test_agg_pow_stri32_bool_max_zero");
  let a1 = false;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_max_normal() {
  dump("testing: test_agg_pow_stri32_bool_max_normal");
  let a1 = true;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_max_min() {
  dump("testing: test_agg_pow_stri32_bool_max_min");
  let a1 = false;
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_zero_max() {
  dump("testing: test_agg_pow_stri32_bool_zero_max");
  let a1 = true;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_zero_zero() {
  dump("testing: test_agg_pow_stri32_bool_zero_zero");
  let a1 = false;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_zero_normal() {
  dump("testing: test_agg_pow_stri32_bool_zero_normal");
  let a1 = true;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_zero_min() {
  dump("testing: test_agg_pow_stri32_bool_zero_min");
  let a1 = false;
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_normal_max() {
  dump("testing: test_agg_pow_stri32_bool_normal_max");
  let a1 = true;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_normal_zero() {
  dump("testing: test_agg_pow_stri32_bool_normal_zero");
  let a1 = false;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_normal_normal() {
  dump("testing: test_agg_pow_stri32_bool_normal_normal");
  let a1 = true;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_normal_min() {
  dump("testing: test_agg_pow_stri32_bool_normal_min");
  let a1 = false;
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_min_max() {
  dump("testing: test_agg_pow_stri32_bool_min_max");
  let a1 = true;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_min_zero() {
  dump("testing: test_agg_pow_stri32_bool_min_zero");
  let a1 = false;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_min_normal() {
  dump("testing: test_agg_pow_stri32_bool_min_normal");
  let a1 = true;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_bool_min_min() {
  dump("testing: test_agg_pow_stri32_bool_min_min");
  let a1 = false;
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_max_max() {
  dump("testing: test_agg_pow_stri32_object_max_max");
  let a1 = {'a':1};
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_max_zero() {
  dump("testing: test_agg_pow_stri32_object_max_zero");
  let a1 = {};
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_max_normal() {
  dump("testing: test_agg_pow_stri32_object_max_normal");
  let a1 = {'a':100};
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_max_min() {
  dump("testing: test_agg_pow_stri32_object_max_min");
  let a1 = {};
  let test_value = '2147483647';
  let original = '2147483647';

  let expect = '2147483647';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_zero_max() {
  dump("testing: test_agg_pow_stri32_object_zero_max");
  let a1 = {'a':1};
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_zero_zero() {
  dump("testing: test_agg_pow_stri32_object_zero_zero");
  let a1 = {};
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_zero_normal() {
  dump("testing: test_agg_pow_stri32_object_zero_normal");
  let a1 = {'a':100};
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_zero_min() {
  dump("testing: test_agg_pow_stri32_object_zero_min");
  let a1 = {};
  let test_value = '1';
  let original = '1';

  let expect = '1';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_normal_max() {
  dump("testing: test_agg_pow_stri32_object_normal_max");
  let a1 = {'a':1};
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_normal_zero() {
  dump("testing: test_agg_pow_stri32_object_normal_zero");
  let a1 = {};
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_normal_normal() {
  dump("testing: test_agg_pow_stri32_object_normal_normal");
  let a1 = {'a':100};
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_normal_min() {
  dump("testing: test_agg_pow_stri32_object_normal_min");
  let a1 = {};
  let test_value = '1000';
  let original = '1000';

  let expect = '1000';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_min_max() {
  dump("testing: test_agg_pow_stri32_object_min_max");
  let a1 = {'a':1};
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_min_zero() {
  dump("testing: test_agg_pow_stri32_object_min_zero");
  let a1 = {};
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_min_normal() {
  dump("testing: test_agg_pow_stri32_object_min_normal");
  let a1 = {'a':100};
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_stri32_object_min_min() {
  dump("testing: test_agg_pow_stri32_object_min_min");
  let a1 = {};
  let test_value = '-2147483648';
  let original = '-2147483648';

  let expect = '-2147483648';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_normal() {
  dump("testing: test_agg_pow_strf64_f64_max_normal");
  let a1 = 13.0;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_max() {
  dump("testing: test_agg_pow_strf64_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_min() {
  dump("testing: test_agg_pow_strf64_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_nan() {
  dump("testing: test_agg_pow_strf64_f64_max_nan");
  let a1 = NaN;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_pinf() {
  dump("testing: test_agg_pow_strf64_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_zero() {
  dump("testing: test_agg_pow_strf64_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_max_ninf() {
  dump("testing: test_agg_pow_strf64_f64_max_ninf");
  let a1 = Infinity;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_normal() {
  dump("testing: test_agg_pow_strf64_f64_zero_normal");
  let a1 = 13.0;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_max() {
  dump("testing: test_agg_pow_strf64_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_min() {
  dump("testing: test_agg_pow_strf64_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_nan() {
  dump("testing: test_agg_pow_strf64_f64_zero_nan");
  let a1 = NaN;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_pinf() {
  dump("testing: test_agg_pow_strf64_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_zero() {
  dump("testing: test_agg_pow_strf64_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_zero_ninf() {
  dump("testing: test_agg_pow_strf64_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_normal() {
  dump("testing: test_agg_pow_strf64_f64_normal_normal");
  let a1 = 13.0;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_max() {
  dump("testing: test_agg_pow_strf64_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_min() {
  dump("testing: test_agg_pow_strf64_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_nan() {
  dump("testing: test_agg_pow_strf64_f64_normal_nan");
  let a1 = NaN;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_pinf() {
  dump("testing: test_agg_pow_strf64_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_zero() {
  dump("testing: test_agg_pow_strf64_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_normal_ninf() {
  dump("testing: test_agg_pow_strf64_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_normal() {
  dump("testing: test_agg_pow_strf64_f64_min_normal");
  let a1 = 13.0;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_max() {
  dump("testing: test_agg_pow_strf64_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_min() {
  dump("testing: test_agg_pow_strf64_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_nan() {
  dump("testing: test_agg_pow_strf64_f64_min_nan");
  let a1 = NaN;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_pinf() {
  dump("testing: test_agg_pow_strf64_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_zero() {
  dump("testing: test_agg_pow_strf64_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_f64_min_ninf() {
  dump("testing: test_agg_pow_strf64_f64_min_ninf");
  let a1 = Infinity;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_max_max() {
  dump("testing: test_agg_pow_strf64_i32_max_max");
  let a1 = 2147483647;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_max_zero() {
  dump("testing: test_agg_pow_strf64_i32_max_zero");
  let a1 = 0;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_max_normal() {
  dump("testing: test_agg_pow_strf64_i32_max_normal");
  let a1 = 14;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_max_min() {
  dump("testing: test_agg_pow_strf64_i32_max_min");
  let a1 = -2147483648;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_zero_max() {
  dump("testing: test_agg_pow_strf64_i32_zero_max");
  let a1 = 2147483647;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_zero_zero() {
  dump("testing: test_agg_pow_strf64_i32_zero_zero");
  let a1 = 0;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_zero_normal() {
  dump("testing: test_agg_pow_strf64_i32_zero_normal");
  let a1 = 14;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_zero_min() {
  dump("testing: test_agg_pow_strf64_i32_zero_min");
  let a1 = -2147483648;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_normal_max() {
  dump("testing: test_agg_pow_strf64_i32_normal_max");
  let a1 = 2147483647;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_normal_zero() {
  dump("testing: test_agg_pow_strf64_i32_normal_zero");
  let a1 = 0;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_normal_normal() {
  dump("testing: test_agg_pow_strf64_i32_normal_normal");
  let a1 = 14;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_normal_min() {
  dump("testing: test_agg_pow_strf64_i32_normal_min");
  let a1 = -2147483648;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_min_max() {
  dump("testing: test_agg_pow_strf64_i32_min_max");
  let a1 = 2147483647;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_min_zero() {
  dump("testing: test_agg_pow_strf64_i32_min_zero");
  let a1 = 0;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_min_normal() {
  dump("testing: test_agg_pow_strf64_i32_min_normal");
  let a1 = 14;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_i32_min_min() {
  dump("testing: test_agg_pow_strf64_i32_min_min");
  let a1 = -2147483648;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_max_max() {
  dump("testing: test_agg_pow_strf64_stri32_max_max");
  let a1 = '2147483647';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_max_zero() {
  dump("testing: test_agg_pow_strf64_stri32_max_zero");
  let a1 = '1';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_max_normal() {
  dump("testing: test_agg_pow_strf64_stri32_max_normal");
  let a1 = '1000';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_max_min() {
  dump("testing: test_agg_pow_strf64_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_zero_max() {
  dump("testing: test_agg_pow_strf64_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_zero_zero() {
  dump("testing: test_agg_pow_strf64_stri32_zero_zero");
  let a1 = '1';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_zero_normal() {
  dump("testing: test_agg_pow_strf64_stri32_zero_normal");
  let a1 = '1000';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_zero_min() {
  dump("testing: test_agg_pow_strf64_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_normal_max() {
  dump("testing: test_agg_pow_strf64_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_normal_zero() {
  dump("testing: test_agg_pow_strf64_stri32_normal_zero");
  let a1 = '1';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_normal_normal() {
  dump("testing: test_agg_pow_strf64_stri32_normal_normal");
  let a1 = '1000';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_normal_min() {
  dump("testing: test_agg_pow_strf64_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_min_max() {
  dump("testing: test_agg_pow_strf64_stri32_min_max");
  let a1 = '2147483647';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_min_zero() {
  dump("testing: test_agg_pow_strf64_stri32_min_zero");
  let a1 = '1';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_min_normal() {
  dump("testing: test_agg_pow_strf64_stri32_min_normal");
  let a1 = '1000';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_stri32_min_min() {
  dump("testing: test_agg_pow_strf64_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_max_max() {
  dump("testing: test_agg_pow_strf64_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_max_zero() {
  dump("testing: test_agg_pow_strf64_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_max_normal() {
  dump("testing: test_agg_pow_strf64_strf64_max_normal");
  let a1 = '13.2';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_max_min() {
  dump("testing: test_agg_pow_strf64_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_zero_max() {
  dump("testing: test_agg_pow_strf64_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_zero_zero() {
  dump("testing: test_agg_pow_strf64_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_zero_normal() {
  dump("testing: test_agg_pow_strf64_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_zero_min() {
  dump("testing: test_agg_pow_strf64_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_normal_max() {
  dump("testing: test_agg_pow_strf64_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_normal_zero() {
  dump("testing: test_agg_pow_strf64_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_normal_normal() {
  dump("testing: test_agg_pow_strf64_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_normal_min() {
  dump("testing: test_agg_pow_strf64_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_min_max() {
  dump("testing: test_agg_pow_strf64_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_min_zero() {
  dump("testing: test_agg_pow_strf64_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_min_normal() {
  dump("testing: test_agg_pow_strf64_strf64_min_normal");
  let a1 = '13.2';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_strf64_min_min() {
  dump("testing: test_agg_pow_strf64_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_max_max() {
  dump("testing: test_agg_pow_strf64_bool_max_max");
  let a1 = true;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_max_zero() {
  dump("testing: test_agg_pow_strf64_bool_max_zero");
  let a1 = false;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_max_normal() {
  dump("testing: test_agg_pow_strf64_bool_max_normal");
  let a1 = true;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_max_min() {
  dump("testing: test_agg_pow_strf64_bool_max_min");
  let a1 = false;
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_zero_max() {
  dump("testing: test_agg_pow_strf64_bool_zero_max");
  let a1 = true;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_zero_zero() {
  dump("testing: test_agg_pow_strf64_bool_zero_zero");
  let a1 = false;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_zero_normal() {
  dump("testing: test_agg_pow_strf64_bool_zero_normal");
  let a1 = true;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_zero_min() {
  dump("testing: test_agg_pow_strf64_bool_zero_min");
  let a1 = false;
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_normal_max() {
  dump("testing: test_agg_pow_strf64_bool_normal_max");
  let a1 = true;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_normal_zero() {
  dump("testing: test_agg_pow_strf64_bool_normal_zero");
  let a1 = false;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_normal_normal() {
  dump("testing: test_agg_pow_strf64_bool_normal_normal");
  let a1 = true;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_normal_min() {
  dump("testing: test_agg_pow_strf64_bool_normal_min");
  let a1 = false;
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_min_max() {
  dump("testing: test_agg_pow_strf64_bool_min_max");
  let a1 = true;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_min_zero() {
  dump("testing: test_agg_pow_strf64_bool_min_zero");
  let a1 = false;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_min_normal() {
  dump("testing: test_agg_pow_strf64_bool_min_normal");
  let a1 = true;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_bool_min_min() {
  dump("testing: test_agg_pow_strf64_bool_min_min");
  let a1 = false;
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_max_max() {
  dump("testing: test_agg_pow_strf64_object_max_max");
  let a1 = {'a':1};
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_max_zero() {
  dump("testing: test_agg_pow_strf64_object_max_zero");
  let a1 = {};
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_max_normal() {
  dump("testing: test_agg_pow_strf64_object_max_normal");
  let a1 = {'a':100};
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_max_min() {
  dump("testing: test_agg_pow_strf64_object_max_min");
  let a1 = {};
  let test_value = '1.79769313486231570e+308';
  let original = '1.79769313486231570e+308';

  let expect = '1.79769313486231570e+308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_zero_max() {
  dump("testing: test_agg_pow_strf64_object_zero_max");
  let a1 = {'a':1};
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_zero_zero() {
  dump("testing: test_agg_pow_strf64_object_zero_zero");
  let a1 = {};
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_zero_normal() {
  dump("testing: test_agg_pow_strf64_object_zero_normal");
  let a1 = {'a':100};
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_zero_min() {
  dump("testing: test_agg_pow_strf64_object_zero_min");
  let a1 = {};
  let test_value = '2.2250738585072014E-308';
  let original = '2.2250738585072014E-308';

  let expect = '2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_normal_max() {
  dump("testing: test_agg_pow_strf64_object_normal_max");
  let a1 = {'a':1};
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_normal_zero() {
  dump("testing: test_agg_pow_strf64_object_normal_zero");
  let a1 = {};
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_normal_normal() {
  dump("testing: test_agg_pow_strf64_object_normal_normal");
  let a1 = {'a':100};
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_normal_min() {
  dump("testing: test_agg_pow_strf64_object_normal_min");
  let a1 = {};
  let test_value = '13.2';
  let original = '13.2';

  let expect = '13.2';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_min_max() {
  dump("testing: test_agg_pow_strf64_object_min_max");
  let a1 = {'a':1};
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_min_zero() {
  dump("testing: test_agg_pow_strf64_object_min_zero");
  let a1 = {};
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_min_normal() {
  dump("testing: test_agg_pow_strf64_object_min_normal");
  let a1 = {'a':100};
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_strf64_object_min_min() {
  dump("testing: test_agg_pow_strf64_object_min_min");
  let a1 = {};
  let test_value = '-2.2250738585072014E-308';
  let original = '-2.2250738585072014E-308';

  let expect = '-2.2250738585072014E-308';
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_normal() {
  dump("testing: test_agg_pow_bool_f64_max_normal");
  let a1 = 13.0;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_max() {
  dump("testing: test_agg_pow_bool_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_min() {
  dump("testing: test_agg_pow_bool_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_nan() {
  dump("testing: test_agg_pow_bool_f64_max_nan");
  let a1 = NaN;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_pinf() {
  dump("testing: test_agg_pow_bool_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_zero() {
  dump("testing: test_agg_pow_bool_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_max_ninf() {
  dump("testing: test_agg_pow_bool_f64_max_ninf");
  let a1 = Infinity;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_normal() {
  dump("testing: test_agg_pow_bool_f64_zero_normal");
  let a1 = 13.0;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_max() {
  dump("testing: test_agg_pow_bool_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_min() {
  dump("testing: test_agg_pow_bool_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_nan() {
  dump("testing: test_agg_pow_bool_f64_zero_nan");
  let a1 = NaN;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_pinf() {
  dump("testing: test_agg_pow_bool_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_zero() {
  dump("testing: test_agg_pow_bool_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_zero_ninf() {
  dump("testing: test_agg_pow_bool_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_normal() {
  dump("testing: test_agg_pow_bool_f64_normal_normal");
  let a1 = 13.0;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_max() {
  dump("testing: test_agg_pow_bool_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_min() {
  dump("testing: test_agg_pow_bool_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_nan() {
  dump("testing: test_agg_pow_bool_f64_normal_nan");
  let a1 = NaN;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_pinf() {
  dump("testing: test_agg_pow_bool_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_zero() {
  dump("testing: test_agg_pow_bool_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_normal_ninf() {
  dump("testing: test_agg_pow_bool_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_normal() {
  dump("testing: test_agg_pow_bool_f64_min_normal");
  let a1 = 13.0;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_max() {
  dump("testing: test_agg_pow_bool_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_min() {
  dump("testing: test_agg_pow_bool_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_nan() {
  dump("testing: test_agg_pow_bool_f64_min_nan");
  let a1 = NaN;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_pinf() {
  dump("testing: test_agg_pow_bool_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_zero() {
  dump("testing: test_agg_pow_bool_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_f64_min_ninf() {
  dump("testing: test_agg_pow_bool_f64_min_ninf");
  let a1 = Infinity;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_max_max() {
  dump("testing: test_agg_pow_bool_i32_max_max");
  let a1 = 2147483647;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_max_zero() {
  dump("testing: test_agg_pow_bool_i32_max_zero");
  let a1 = 0;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_max_normal() {
  dump("testing: test_agg_pow_bool_i32_max_normal");
  let a1 = 14;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_max_min() {
  dump("testing: test_agg_pow_bool_i32_max_min");
  let a1 = -2147483648;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_zero_max() {
  dump("testing: test_agg_pow_bool_i32_zero_max");
  let a1 = 2147483647;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_zero_zero() {
  dump("testing: test_agg_pow_bool_i32_zero_zero");
  let a1 = 0;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_zero_normal() {
  dump("testing: test_agg_pow_bool_i32_zero_normal");
  let a1 = 14;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_zero_min() {
  dump("testing: test_agg_pow_bool_i32_zero_min");
  let a1 = -2147483648;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_normal_max() {
  dump("testing: test_agg_pow_bool_i32_normal_max");
  let a1 = 2147483647;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_normal_zero() {
  dump("testing: test_agg_pow_bool_i32_normal_zero");
  let a1 = 0;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_normal_normal() {
  dump("testing: test_agg_pow_bool_i32_normal_normal");
  let a1 = 14;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_normal_min() {
  dump("testing: test_agg_pow_bool_i32_normal_min");
  let a1 = -2147483648;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_min_max() {
  dump("testing: test_agg_pow_bool_i32_min_max");
  let a1 = 2147483647;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_min_zero() {
  dump("testing: test_agg_pow_bool_i32_min_zero");
  let a1 = 0;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_min_normal() {
  dump("testing: test_agg_pow_bool_i32_min_normal");
  let a1 = 14;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_i32_min_min() {
  dump("testing: test_agg_pow_bool_i32_min_min");
  let a1 = -2147483648;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_max_max() {
  dump("testing: test_agg_pow_bool_stri32_max_max");
  let a1 = '2147483647';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_max_zero() {
  dump("testing: test_agg_pow_bool_stri32_max_zero");
  let a1 = '1';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_max_normal() {
  dump("testing: test_agg_pow_bool_stri32_max_normal");
  let a1 = '1000';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_max_min() {
  dump("testing: test_agg_pow_bool_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_zero_max() {
  dump("testing: test_agg_pow_bool_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_zero_zero() {
  dump("testing: test_agg_pow_bool_stri32_zero_zero");
  let a1 = '1';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_zero_normal() {
  dump("testing: test_agg_pow_bool_stri32_zero_normal");
  let a1 = '1000';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_zero_min() {
  dump("testing: test_agg_pow_bool_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_normal_max() {
  dump("testing: test_agg_pow_bool_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_normal_zero() {
  dump("testing: test_agg_pow_bool_stri32_normal_zero");
  let a1 = '1';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_normal_normal() {
  dump("testing: test_agg_pow_bool_stri32_normal_normal");
  let a1 = '1000';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_normal_min() {
  dump("testing: test_agg_pow_bool_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_min_max() {
  dump("testing: test_agg_pow_bool_stri32_min_max");
  let a1 = '2147483647';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_min_zero() {
  dump("testing: test_agg_pow_bool_stri32_min_zero");
  let a1 = '1';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_min_normal() {
  dump("testing: test_agg_pow_bool_stri32_min_normal");
  let a1 = '1000';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_stri32_min_min() {
  dump("testing: test_agg_pow_bool_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_max_max() {
  dump("testing: test_agg_pow_bool_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_max_zero() {
  dump("testing: test_agg_pow_bool_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_max_normal() {
  dump("testing: test_agg_pow_bool_strf64_max_normal");
  let a1 = '13.2';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_max_min() {
  dump("testing: test_agg_pow_bool_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_zero_max() {
  dump("testing: test_agg_pow_bool_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_zero_zero() {
  dump("testing: test_agg_pow_bool_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_zero_normal() {
  dump("testing: test_agg_pow_bool_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_zero_min() {
  dump("testing: test_agg_pow_bool_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_normal_max() {
  dump("testing: test_agg_pow_bool_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_normal_zero() {
  dump("testing: test_agg_pow_bool_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_normal_normal() {
  dump("testing: test_agg_pow_bool_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_normal_min() {
  dump("testing: test_agg_pow_bool_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_min_max() {
  dump("testing: test_agg_pow_bool_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_min_zero() {
  dump("testing: test_agg_pow_bool_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_min_normal() {
  dump("testing: test_agg_pow_bool_strf64_min_normal");
  let a1 = '13.2';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_strf64_min_min() {
  dump("testing: test_agg_pow_bool_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_max_max() {
  dump("testing: test_agg_pow_bool_bool_max_max");
  let a1 = true;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_max_zero() {
  dump("testing: test_agg_pow_bool_bool_max_zero");
  let a1 = false;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_max_normal() {
  dump("testing: test_agg_pow_bool_bool_max_normal");
  let a1 = true;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_max_min() {
  dump("testing: test_agg_pow_bool_bool_max_min");
  let a1 = false;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_zero_max() {
  dump("testing: test_agg_pow_bool_bool_zero_max");
  let a1 = true;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_zero_zero() {
  dump("testing: test_agg_pow_bool_bool_zero_zero");
  let a1 = false;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_zero_normal() {
  dump("testing: test_agg_pow_bool_bool_zero_normal");
  let a1 = true;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_zero_min() {
  dump("testing: test_agg_pow_bool_bool_zero_min");
  let a1 = false;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_normal_max() {
  dump("testing: test_agg_pow_bool_bool_normal_max");
  let a1 = true;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_normal_zero() {
  dump("testing: test_agg_pow_bool_bool_normal_zero");
  let a1 = false;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_normal_normal() {
  dump("testing: test_agg_pow_bool_bool_normal_normal");
  let a1 = true;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_normal_min() {
  dump("testing: test_agg_pow_bool_bool_normal_min");
  let a1 = false;
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_min_max() {
  dump("testing: test_agg_pow_bool_bool_min_max");
  let a1 = true;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_min_zero() {
  dump("testing: test_agg_pow_bool_bool_min_zero");
  let a1 = false;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_min_normal() {
  dump("testing: test_agg_pow_bool_bool_min_normal");
  let a1 = true;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_bool_min_min() {
  dump("testing: test_agg_pow_bool_bool_min_min");
  let a1 = false;
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_max_max() {
  dump("testing: test_agg_pow_bool_object_max_max");
  let a1 = {'a':1};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_max_zero() {
  dump("testing: test_agg_pow_bool_object_max_zero");
  let a1 = {};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_max_normal() {
  dump("testing: test_agg_pow_bool_object_max_normal");
  let a1 = {'a':100};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_max_min() {
  dump("testing: test_agg_pow_bool_object_max_min");
  let a1 = {};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_zero_max() {
  dump("testing: test_agg_pow_bool_object_zero_max");
  let a1 = {'a':1};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_zero_zero() {
  dump("testing: test_agg_pow_bool_object_zero_zero");
  let a1 = {};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_zero_normal() {
  dump("testing: test_agg_pow_bool_object_zero_normal");
  let a1 = {'a':100};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_zero_min() {
  dump("testing: test_agg_pow_bool_object_zero_min");
  let a1 = {};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_normal_max() {
  dump("testing: test_agg_pow_bool_object_normal_max");
  let a1 = {'a':1};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_normal_zero() {
  dump("testing: test_agg_pow_bool_object_normal_zero");
  let a1 = {};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_normal_normal() {
  dump("testing: test_agg_pow_bool_object_normal_normal");
  let a1 = {'a':100};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_normal_min() {
  dump("testing: test_agg_pow_bool_object_normal_min");
  let a1 = {};
  let test_value = true;
  let original = true;

  let expect = true;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_min_max() {
  dump("testing: test_agg_pow_bool_object_min_max");
  let a1 = {'a':1};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_min_zero() {
  dump("testing: test_agg_pow_bool_object_min_zero");
  let a1 = {};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_min_normal() {
  dump("testing: test_agg_pow_bool_object_min_normal");
  let a1 = {'a':100};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_bool_object_min_min() {
  dump("testing: test_agg_pow_bool_object_min_min");
  let a1 = {};
  let test_value = false;
  let original = false;

  let expect = false;
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_normal() {
  dump("testing: test_agg_pow_object_f64_max_normal");
  let a1 = 13.0;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_max() {
  dump("testing: test_agg_pow_object_f64_max_max");
  let a1 = 1.79769313486e+308;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_min() {
  dump("testing: test_agg_pow_object_f64_max_min");
  let a1 = -1.79769313486e+308;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_nan() {
  dump("testing: test_agg_pow_object_f64_max_nan");
  let a1 = NaN;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_pinf() {
  dump("testing: test_agg_pow_object_f64_max_pinf");
  let a1 = -Infinity;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_zero() {
  dump("testing: test_agg_pow_object_f64_max_zero");
  let a1 = 2.22507385851e-308;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_max_ninf() {
  dump("testing: test_agg_pow_object_f64_max_ninf");
  let a1 = Infinity;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_normal() {
  dump("testing: test_agg_pow_object_f64_zero_normal");
  let a1 = 13.0;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_max() {
  dump("testing: test_agg_pow_object_f64_zero_max");
  let a1 = 1.79769313486e+308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_min() {
  dump("testing: test_agg_pow_object_f64_zero_min");
  let a1 = -1.79769313486e+308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_nan() {
  dump("testing: test_agg_pow_object_f64_zero_nan");
  let a1 = NaN;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_pinf() {
  dump("testing: test_agg_pow_object_f64_zero_pinf");
  let a1 = -Infinity;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_zero() {
  dump("testing: test_agg_pow_object_f64_zero_zero");
  let a1 = 2.22507385851e-308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_zero_ninf() {
  dump("testing: test_agg_pow_object_f64_zero_ninf");
  let a1 = Infinity;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_normal() {
  dump("testing: test_agg_pow_object_f64_normal_normal");
  let a1 = 13.0;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_max() {
  dump("testing: test_agg_pow_object_f64_normal_max");
  let a1 = 1.79769313486e+308;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_min() {
  dump("testing: test_agg_pow_object_f64_normal_min");
  let a1 = -1.79769313486e+308;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_nan() {
  dump("testing: test_agg_pow_object_f64_normal_nan");
  let a1 = NaN;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_pinf() {
  dump("testing: test_agg_pow_object_f64_normal_pinf");
  let a1 = -Infinity;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_zero() {
  dump("testing: test_agg_pow_object_f64_normal_zero");
  let a1 = 2.22507385851e-308;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_normal_ninf() {
  dump("testing: test_agg_pow_object_f64_normal_ninf");
  let a1 = Infinity;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_normal() {
  dump("testing: test_agg_pow_object_f64_min_normal");
  let a1 = 13.0;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_max() {
  dump("testing: test_agg_pow_object_f64_min_max");
  let a1 = 1.79769313486e+308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_min() {
  dump("testing: test_agg_pow_object_f64_min_min");
  let a1 = -1.79769313486e+308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_nan() {
  dump("testing: test_agg_pow_object_f64_min_nan");
  let a1 = NaN;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_pinf() {
  dump("testing: test_agg_pow_object_f64_min_pinf");
  let a1 = -Infinity;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_zero() {
  dump("testing: test_agg_pow_object_f64_min_zero");
  let a1 = 2.22507385851e-308;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_f64_min_ninf() {
  dump("testing: test_agg_pow_object_f64_min_ninf");
  let a1 = Infinity;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_max_max() {
  dump("testing: test_agg_pow_object_i32_max_max");
  let a1 = 2147483647;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_max_zero() {
  dump("testing: test_agg_pow_object_i32_max_zero");
  let a1 = 0;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_max_normal() {
  dump("testing: test_agg_pow_object_i32_max_normal");
  let a1 = 14;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_max_min() {
  dump("testing: test_agg_pow_object_i32_max_min");
  let a1 = -2147483648;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_zero_max() {
  dump("testing: test_agg_pow_object_i32_zero_max");
  let a1 = 2147483647;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_zero_zero() {
  dump("testing: test_agg_pow_object_i32_zero_zero");
  let a1 = 0;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_zero_normal() {
  dump("testing: test_agg_pow_object_i32_zero_normal");
  let a1 = 14;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_zero_min() {
  dump("testing: test_agg_pow_object_i32_zero_min");
  let a1 = -2147483648;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_normal_max() {
  dump("testing: test_agg_pow_object_i32_normal_max");
  let a1 = 2147483647;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_normal_zero() {
  dump("testing: test_agg_pow_object_i32_normal_zero");
  let a1 = 0;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_normal_normal() {
  dump("testing: test_agg_pow_object_i32_normal_normal");
  let a1 = 14;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_normal_min() {
  dump("testing: test_agg_pow_object_i32_normal_min");
  let a1 = -2147483648;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_min_max() {
  dump("testing: test_agg_pow_object_i32_min_max");
  let a1 = 2147483647;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_min_zero() {
  dump("testing: test_agg_pow_object_i32_min_zero");
  let a1 = 0;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_min_normal() {
  dump("testing: test_agg_pow_object_i32_min_normal");
  let a1 = 14;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_i32_min_min() {
  dump("testing: test_agg_pow_object_i32_min_min");
  let a1 = -2147483648;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_max_max() {
  dump("testing: test_agg_pow_object_stri32_max_max");
  let a1 = '2147483647';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_max_zero() {
  dump("testing: test_agg_pow_object_stri32_max_zero");
  let a1 = '1';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_max_normal() {
  dump("testing: test_agg_pow_object_stri32_max_normal");
  let a1 = '1000';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_max_min() {
  dump("testing: test_agg_pow_object_stri32_max_min");
  let a1 = '-2147483648';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_zero_max() {
  dump("testing: test_agg_pow_object_stri32_zero_max");
  let a1 = '2147483647';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_zero_zero() {
  dump("testing: test_agg_pow_object_stri32_zero_zero");
  let a1 = '1';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_zero_normal() {
  dump("testing: test_agg_pow_object_stri32_zero_normal");
  let a1 = '1000';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_zero_min() {
  dump("testing: test_agg_pow_object_stri32_zero_min");
  let a1 = '-2147483648';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_normal_max() {
  dump("testing: test_agg_pow_object_stri32_normal_max");
  let a1 = '2147483647';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_normal_zero() {
  dump("testing: test_agg_pow_object_stri32_normal_zero");
  let a1 = '1';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_normal_normal() {
  dump("testing: test_agg_pow_object_stri32_normal_normal");
  let a1 = '1000';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_normal_min() {
  dump("testing: test_agg_pow_object_stri32_normal_min");
  let a1 = '-2147483648';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_min_max() {
  dump("testing: test_agg_pow_object_stri32_min_max");
  let a1 = '2147483647';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_min_zero() {
  dump("testing: test_agg_pow_object_stri32_min_zero");
  let a1 = '1';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_min_normal() {
  dump("testing: test_agg_pow_object_stri32_min_normal");
  let a1 = '1000';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_stri32_min_min() {
  dump("testing: test_agg_pow_object_stri32_min_min");
  let a1 = '-2147483648';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_max_max() {
  dump("testing: test_agg_pow_object_strf64_max_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_max_zero() {
  dump("testing: test_agg_pow_object_strf64_max_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_max_normal() {
  dump("testing: test_agg_pow_object_strf64_max_normal");
  let a1 = '13.2';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_max_min() {
  dump("testing: test_agg_pow_object_strf64_max_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_zero_max() {
  dump("testing: test_agg_pow_object_strf64_zero_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_zero_zero() {
  dump("testing: test_agg_pow_object_strf64_zero_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_zero_normal() {
  dump("testing: test_agg_pow_object_strf64_zero_normal");
  let a1 = '13.2';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_zero_min() {
  dump("testing: test_agg_pow_object_strf64_zero_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_normal_max() {
  dump("testing: test_agg_pow_object_strf64_normal_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_normal_zero() {
  dump("testing: test_agg_pow_object_strf64_normal_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_normal_normal() {
  dump("testing: test_agg_pow_object_strf64_normal_normal");
  let a1 = '13.2';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_normal_min() {
  dump("testing: test_agg_pow_object_strf64_normal_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_min_max() {
  dump("testing: test_agg_pow_object_strf64_min_max");
  let a1 = '1.79769313486231570e+308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_min_zero() {
  dump("testing: test_agg_pow_object_strf64_min_zero");
  let a1 = '2.2250738585072014E-308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_min_normal() {
  dump("testing: test_agg_pow_object_strf64_min_normal");
  let a1 = '13.2';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_strf64_min_min() {
  dump("testing: test_agg_pow_object_strf64_min_min");
  let a1 = '-2.2250738585072014E-308';
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_max_max() {
  dump("testing: test_agg_pow_object_bool_max_max");
  let a1 = true;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_max_zero() {
  dump("testing: test_agg_pow_object_bool_max_zero");
  let a1 = false;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_max_normal() {
  dump("testing: test_agg_pow_object_bool_max_normal");
  let a1 = true;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_max_min() {
  dump("testing: test_agg_pow_object_bool_max_min");
  let a1 = false;
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_zero_max() {
  dump("testing: test_agg_pow_object_bool_zero_max");
  let a1 = true;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_zero_zero() {
  dump("testing: test_agg_pow_object_bool_zero_zero");
  let a1 = false;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_zero_normal() {
  dump("testing: test_agg_pow_object_bool_zero_normal");
  let a1 = true;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_zero_min() {
  dump("testing: test_agg_pow_object_bool_zero_min");
  let a1 = false;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_normal_max() {
  dump("testing: test_agg_pow_object_bool_normal_max");
  let a1 = true;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_normal_zero() {
  dump("testing: test_agg_pow_object_bool_normal_zero");
  let a1 = false;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_normal_normal() {
  dump("testing: test_agg_pow_object_bool_normal_normal");
  let a1 = true;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_normal_min() {
  dump("testing: test_agg_pow_object_bool_normal_min");
  let a1 = false;
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_min_max() {
  dump("testing: test_agg_pow_object_bool_min_max");
  let a1 = true;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_min_zero() {
  dump("testing: test_agg_pow_object_bool_min_zero");
  let a1 = false;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_min_normal() {
  dump("testing: test_agg_pow_object_bool_min_normal");
  let a1 = true;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_bool_min_min() {
  dump("testing: test_agg_pow_object_bool_min_min");
  let a1 = false;
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_max_max() {
  dump("testing: test_agg_pow_object_object_max_max");
  let a1 = {'a':1};
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_max_zero() {
  dump("testing: test_agg_pow_object_object_max_zero");
  let a1 = {};
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_max_normal() {
  dump("testing: test_agg_pow_object_object_max_normal");
  let a1 = {'a':100};
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_max_min() {
  dump("testing: test_agg_pow_object_object_max_min");
  let a1 = {};
  let test_value = {'a':1};
  let original = {'a':1};

  let expect = {'a':1};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_zero_max() {
  dump("testing: test_agg_pow_object_object_zero_max");
  let a1 = {'a':1};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_zero_zero() {
  dump("testing: test_agg_pow_object_object_zero_zero");
  let a1 = {};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_zero_normal() {
  dump("testing: test_agg_pow_object_object_zero_normal");
  let a1 = {'a':100};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_zero_min() {
  dump("testing: test_agg_pow_object_object_zero_min");
  let a1 = {};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_normal_max() {
  dump("testing: test_agg_pow_object_object_normal_max");
  let a1 = {'a':1};
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_normal_zero() {
  dump("testing: test_agg_pow_object_object_normal_zero");
  let a1 = {};
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_normal_normal() {
  dump("testing: test_agg_pow_object_object_normal_normal");
  let a1 = {'a':100};
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_normal_min() {
  dump("testing: test_agg_pow_object_object_normal_min");
  let a1 = {};
  let test_value = {'a':100};
  let original = {'a':100};

  let expect = {'a':100};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_min_max() {
  dump("testing: test_agg_pow_object_object_min_max");
  let a1 = {'a':1};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_min_zero() {
  dump("testing: test_agg_pow_object_object_min_zero");
  let a1 = {};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_min_normal() {
  dump("testing: test_agg_pow_object_object_min_normal");
  let a1 = {'a':100};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_agg_pow_object_object_min_min() {
  dump("testing: test_agg_pow_object_object_min_min");
  let a1 = {};
  let test_value = {};
  let original = {};

  let expect = {};
  expect **= a1;

  for (let i = 0; i < 50; i++) {
    let temp = original;
    temp **= a1;
    test_value = temp;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}
  test_agg_pow_f64_f64_normal_normal();
  test_agg_pow_f64_f64_normal_max();
  test_agg_pow_f64_f64_normal_min();
  test_agg_pow_f64_f64_normal_nan();
  test_agg_pow_f64_f64_normal_pinf();
  test_agg_pow_f64_f64_normal_zero();
  test_agg_pow_f64_f64_normal_ninf();
  test_agg_pow_f64_f64_max_normal();
  test_agg_pow_f64_f64_max_max();
  test_agg_pow_f64_f64_max_min();
  test_agg_pow_f64_f64_max_nan();
  test_agg_pow_f64_f64_max_pinf();
  test_agg_pow_f64_f64_max_zero();
  test_agg_pow_f64_f64_max_ninf();
  test_agg_pow_f64_f64_min_normal();
  test_agg_pow_f64_f64_min_max();
  test_agg_pow_f64_f64_min_min();
  test_agg_pow_f64_f64_min_nan();
  test_agg_pow_f64_f64_min_pinf();
  test_agg_pow_f64_f64_min_zero();
  test_agg_pow_f64_f64_min_ninf();
  test_agg_pow_f64_f64_nan_normal();
  test_agg_pow_f64_f64_nan_max();
  test_agg_pow_f64_f64_nan_min();
  test_agg_pow_f64_f64_nan_nan();
  test_agg_pow_f64_f64_nan_pinf();
  test_agg_pow_f64_f64_nan_zero();
  test_agg_pow_f64_f64_nan_ninf();
  test_agg_pow_f64_f64_pinf_normal();
  test_agg_pow_f64_f64_pinf_max();
  test_agg_pow_f64_f64_pinf_min();
  test_agg_pow_f64_f64_pinf_nan();
  test_agg_pow_f64_f64_pinf_pinf();
  test_agg_pow_f64_f64_pinf_zero();
  test_agg_pow_f64_f64_pinf_ninf();
  test_agg_pow_f64_f64_zero_normal();
  test_agg_pow_f64_f64_zero_max();
  test_agg_pow_f64_f64_zero_min();
  test_agg_pow_f64_f64_zero_nan();
  test_agg_pow_f64_f64_zero_pinf();
  test_agg_pow_f64_f64_zero_zero();
  test_agg_pow_f64_f64_zero_ninf();
  test_agg_pow_f64_f64_ninf_normal();
  test_agg_pow_f64_f64_ninf_max();
  test_agg_pow_f64_f64_ninf_min();
  test_agg_pow_f64_f64_ninf_nan();
  test_agg_pow_f64_f64_ninf_pinf();
  test_agg_pow_f64_f64_ninf_zero();
  test_agg_pow_f64_f64_ninf_ninf();
  test_agg_pow_f64_i32_normal_max();
  test_agg_pow_f64_i32_normal_zero();
  test_agg_pow_f64_i32_normal_normal();
  test_agg_pow_f64_i32_normal_min();
  test_agg_pow_f64_i32_max_max();
  test_agg_pow_f64_i32_max_zero();
  test_agg_pow_f64_i32_max_normal();
  test_agg_pow_f64_i32_max_min();
  test_agg_pow_f64_i32_min_max();
  test_agg_pow_f64_i32_min_zero();
  test_agg_pow_f64_i32_min_normal();
  test_agg_pow_f64_i32_min_min();
  test_agg_pow_f64_i32_nan_max();
  test_agg_pow_f64_i32_nan_zero();
  test_agg_pow_f64_i32_nan_normal();
  test_agg_pow_f64_i32_nan_min();
  test_agg_pow_f64_i32_pinf_max();
  test_agg_pow_f64_i32_pinf_zero();
  test_agg_pow_f64_i32_pinf_normal();
  test_agg_pow_f64_i32_pinf_min();
  test_agg_pow_f64_i32_zero_max();
  test_agg_pow_f64_i32_zero_zero();
  test_agg_pow_f64_i32_zero_normal();
  test_agg_pow_f64_i32_zero_min();
  test_agg_pow_f64_i32_ninf_max();
  test_agg_pow_f64_i32_ninf_zero();
  test_agg_pow_f64_i32_ninf_normal();
  test_agg_pow_f64_i32_ninf_min();
  test_agg_pow_f64_stri32_normal_max();
  test_agg_pow_f64_stri32_normal_zero();
  test_agg_pow_f64_stri32_normal_normal();
  test_agg_pow_f64_stri32_normal_min();
  test_agg_pow_f64_stri32_max_max();
  test_agg_pow_f64_stri32_max_zero();
  test_agg_pow_f64_stri32_max_normal();
  test_agg_pow_f64_stri32_max_min();
  test_agg_pow_f64_stri32_min_max();
  test_agg_pow_f64_stri32_min_zero();
  test_agg_pow_f64_stri32_min_normal();
  test_agg_pow_f64_stri32_min_min();
  test_agg_pow_f64_stri32_nan_max();
  test_agg_pow_f64_stri32_nan_zero();
  test_agg_pow_f64_stri32_nan_normal();
  test_agg_pow_f64_stri32_nan_min();
  test_agg_pow_f64_stri32_pinf_max();
  test_agg_pow_f64_stri32_pinf_zero();
  test_agg_pow_f64_stri32_pinf_normal();
  test_agg_pow_f64_stri32_pinf_min();
  test_agg_pow_f64_stri32_zero_max();
  test_agg_pow_f64_stri32_zero_zero();
  test_agg_pow_f64_stri32_zero_normal();
  test_agg_pow_f64_stri32_zero_min();
  test_agg_pow_f64_stri32_ninf_max();
  test_agg_pow_f64_stri32_ninf_zero();
  test_agg_pow_f64_stri32_ninf_normal();
  test_agg_pow_f64_stri32_ninf_min();
  test_agg_pow_f64_strf64_normal_max();
  test_agg_pow_f64_strf64_normal_zero();
  test_agg_pow_f64_strf64_normal_normal();
  test_agg_pow_f64_strf64_normal_min();
  test_agg_pow_f64_strf64_max_max();
  test_agg_pow_f64_strf64_max_zero();
  test_agg_pow_f64_strf64_max_normal();
  test_agg_pow_f64_strf64_max_min();
  test_agg_pow_f64_strf64_min_max();
  test_agg_pow_f64_strf64_min_zero();
  test_agg_pow_f64_strf64_min_normal();
  test_agg_pow_f64_strf64_min_min();
  test_agg_pow_f64_strf64_nan_max();
  test_agg_pow_f64_strf64_nan_zero();
  test_agg_pow_f64_strf64_nan_normal();
  test_agg_pow_f64_strf64_nan_min();
  test_agg_pow_f64_strf64_pinf_max();
  test_agg_pow_f64_strf64_pinf_zero();
  test_agg_pow_f64_strf64_pinf_normal();
  test_agg_pow_f64_strf64_pinf_min();
  test_agg_pow_f64_strf64_zero_max();
  test_agg_pow_f64_strf64_zero_zero();
  test_agg_pow_f64_strf64_zero_normal();
  test_agg_pow_f64_strf64_zero_min();
  test_agg_pow_f64_strf64_ninf_max();
  test_agg_pow_f64_strf64_ninf_zero();
  test_agg_pow_f64_strf64_ninf_normal();
  test_agg_pow_f64_strf64_ninf_min();
  test_agg_pow_f64_bool_normal_max();
  test_agg_pow_f64_bool_normal_zero();
  test_agg_pow_f64_bool_normal_normal();
  test_agg_pow_f64_bool_normal_min();
  test_agg_pow_f64_bool_max_max();
  test_agg_pow_f64_bool_max_zero();
  test_agg_pow_f64_bool_max_normal();
  test_agg_pow_f64_bool_max_min();
  test_agg_pow_f64_bool_min_max();
  test_agg_pow_f64_bool_min_zero();
  test_agg_pow_f64_bool_min_normal();
  test_agg_pow_f64_bool_min_min();
  test_agg_pow_f64_bool_nan_max();
  test_agg_pow_f64_bool_nan_zero();
  test_agg_pow_f64_bool_nan_normal();
  test_agg_pow_f64_bool_nan_min();
  test_agg_pow_f64_bool_pinf_max();
  test_agg_pow_f64_bool_pinf_zero();
  test_agg_pow_f64_bool_pinf_normal();
  test_agg_pow_f64_bool_pinf_min();
  test_agg_pow_f64_bool_zero_max();
  test_agg_pow_f64_bool_zero_zero();
  test_agg_pow_f64_bool_zero_normal();
  test_agg_pow_f64_bool_zero_min();
  test_agg_pow_f64_bool_ninf_max();
  test_agg_pow_f64_bool_ninf_zero();
  test_agg_pow_f64_bool_ninf_normal();
  test_agg_pow_f64_bool_ninf_min();
  test_agg_pow_f64_object_normal_max();
  test_agg_pow_f64_object_normal_zero();
  test_agg_pow_f64_object_normal_normal();
  test_agg_pow_f64_object_normal_min();
  test_agg_pow_f64_object_max_max();
  test_agg_pow_f64_object_max_zero();
  test_agg_pow_f64_object_max_normal();
  test_agg_pow_f64_object_max_min();
  test_agg_pow_f64_object_min_max();
  test_agg_pow_f64_object_min_zero();
  test_agg_pow_f64_object_min_normal();
  test_agg_pow_f64_object_min_min();
  test_agg_pow_f64_object_nan_max();
  test_agg_pow_f64_object_nan_zero();
  test_agg_pow_f64_object_nan_normal();
  test_agg_pow_f64_object_nan_min();
  test_agg_pow_f64_object_pinf_max();
  test_agg_pow_f64_object_pinf_zero();
  test_agg_pow_f64_object_pinf_normal();
  test_agg_pow_f64_object_pinf_min();
  test_agg_pow_f64_object_zero_max();
  test_agg_pow_f64_object_zero_zero();
  test_agg_pow_f64_object_zero_normal();
  test_agg_pow_f64_object_zero_min();
  test_agg_pow_f64_object_ninf_max();
  test_agg_pow_f64_object_ninf_zero();
  test_agg_pow_f64_object_ninf_normal();
  test_agg_pow_f64_object_ninf_min();
  test_agg_pow_i32_f64_max_normal();
  test_agg_pow_i32_f64_max_max();
  test_agg_pow_i32_f64_max_min();
  test_agg_pow_i32_f64_max_nan();
  test_agg_pow_i32_f64_max_pinf();
  test_agg_pow_i32_f64_max_zero();
  test_agg_pow_i32_f64_max_ninf();
  test_agg_pow_i32_f64_zero_normal();
  test_agg_pow_i32_f64_zero_max();
  test_agg_pow_i32_f64_zero_min();
  test_agg_pow_i32_f64_zero_nan();
  test_agg_pow_i32_f64_zero_pinf();
  test_agg_pow_i32_f64_zero_zero();
  test_agg_pow_i32_f64_zero_ninf();
  test_agg_pow_i32_f64_normal_normal();
  test_agg_pow_i32_f64_normal_max();
  test_agg_pow_i32_f64_normal_min();
  test_agg_pow_i32_f64_normal_nan();
  test_agg_pow_i32_f64_normal_pinf();
  test_agg_pow_i32_f64_normal_zero();
  test_agg_pow_i32_f64_normal_ninf();
  test_agg_pow_i32_f64_min_normal();
  test_agg_pow_i32_f64_min_max();
  test_agg_pow_i32_f64_min_min();
  test_agg_pow_i32_f64_min_nan();
  test_agg_pow_i32_f64_min_pinf();
  test_agg_pow_i32_f64_min_zero();
  test_agg_pow_i32_f64_min_ninf();
  test_agg_pow_i32_i32_max_max();
  test_agg_pow_i32_i32_max_zero();
  test_agg_pow_i32_i32_max_normal();
  test_agg_pow_i32_i32_max_min();
  test_agg_pow_i32_i32_zero_max();
  test_agg_pow_i32_i32_zero_zero();
  test_agg_pow_i32_i32_zero_normal();
  test_agg_pow_i32_i32_zero_min();
  test_agg_pow_i32_i32_normal_max();
  test_agg_pow_i32_i32_normal_zero();
  test_agg_pow_i32_i32_normal_normal();
  test_agg_pow_i32_i32_normal_min();
  test_agg_pow_i32_i32_min_max();
  test_agg_pow_i32_i32_min_zero();
  test_agg_pow_i32_i32_min_normal();
  test_agg_pow_i32_i32_min_min();
  test_agg_pow_i32_stri32_max_max();
  test_agg_pow_i32_stri32_max_zero();
  test_agg_pow_i32_stri32_max_normal();
  test_agg_pow_i32_stri32_max_min();
  test_agg_pow_i32_stri32_zero_max();
  test_agg_pow_i32_stri32_zero_zero();
  test_agg_pow_i32_stri32_zero_normal();
  test_agg_pow_i32_stri32_zero_min();
  test_agg_pow_i32_stri32_normal_max();
  test_agg_pow_i32_stri32_normal_zero();
  test_agg_pow_i32_stri32_normal_normal();
  test_agg_pow_i32_stri32_normal_min();
  test_agg_pow_i32_stri32_min_max();
  test_agg_pow_i32_stri32_min_zero();
  test_agg_pow_i32_stri32_min_normal();
  test_agg_pow_i32_stri32_min_min();
  test_agg_pow_i32_strf64_max_max();
  test_agg_pow_i32_strf64_max_zero();
  test_agg_pow_i32_strf64_max_normal();
  test_agg_pow_i32_strf64_max_min();
  test_agg_pow_i32_strf64_zero_max();
  test_agg_pow_i32_strf64_zero_zero();
  test_agg_pow_i32_strf64_zero_normal();
  test_agg_pow_i32_strf64_zero_min();
  test_agg_pow_i32_strf64_normal_max();
  test_agg_pow_i32_strf64_normal_zero();
  test_agg_pow_i32_strf64_normal_normal();
  test_agg_pow_i32_strf64_normal_min();
  test_agg_pow_i32_strf64_min_max();
  test_agg_pow_i32_strf64_min_zero();
  test_agg_pow_i32_strf64_min_normal();
  test_agg_pow_i32_strf64_min_min();
  test_agg_pow_i32_bool_max_max();
  test_agg_pow_i32_bool_max_zero();
  test_agg_pow_i32_bool_max_normal();
  test_agg_pow_i32_bool_max_min();
  test_agg_pow_i32_bool_zero_max();
  test_agg_pow_i32_bool_zero_zero();
  test_agg_pow_i32_bool_zero_normal();
  test_agg_pow_i32_bool_zero_min();
  test_agg_pow_i32_bool_normal_max();
  test_agg_pow_i32_bool_normal_zero();
  test_agg_pow_i32_bool_normal_normal();
  test_agg_pow_i32_bool_normal_min();
  test_agg_pow_i32_bool_min_max();
  test_agg_pow_i32_bool_min_zero();
  test_agg_pow_i32_bool_min_normal();
  test_agg_pow_i32_bool_min_min();
  test_agg_pow_i32_object_max_max();
  test_agg_pow_i32_object_max_zero();
  test_agg_pow_i32_object_max_normal();
  test_agg_pow_i32_object_max_min();
  test_agg_pow_i32_object_zero_max();
  test_agg_pow_i32_object_zero_zero();
  test_agg_pow_i32_object_zero_normal();
  test_agg_pow_i32_object_zero_min();
  test_agg_pow_i32_object_normal_max();
  test_agg_pow_i32_object_normal_zero();
  test_agg_pow_i32_object_normal_normal();
  test_agg_pow_i32_object_normal_min();
  test_agg_pow_i32_object_min_max();
  test_agg_pow_i32_object_min_zero();
  test_agg_pow_i32_object_min_normal();
  test_agg_pow_i32_object_min_min();
  test_agg_pow_stri32_f64_max_normal();
  test_agg_pow_stri32_f64_max_max();
  test_agg_pow_stri32_f64_max_min();
  test_agg_pow_stri32_f64_max_nan();
  test_agg_pow_stri32_f64_max_pinf();
  test_agg_pow_stri32_f64_max_zero();
  test_agg_pow_stri32_f64_max_ninf();
  test_agg_pow_stri32_f64_zero_normal();
  test_agg_pow_stri32_f64_zero_max();
  test_agg_pow_stri32_f64_zero_min();
  test_agg_pow_stri32_f64_zero_nan();
  test_agg_pow_stri32_f64_zero_pinf();
  test_agg_pow_stri32_f64_zero_zero();
  test_agg_pow_stri32_f64_zero_ninf();
  test_agg_pow_stri32_f64_normal_normal();
  test_agg_pow_stri32_f64_normal_max();
  test_agg_pow_stri32_f64_normal_min();
  test_agg_pow_stri32_f64_normal_nan();
  test_agg_pow_stri32_f64_normal_pinf();
  test_agg_pow_stri32_f64_normal_zero();
  test_agg_pow_stri32_f64_normal_ninf();
  test_agg_pow_stri32_f64_min_normal();
  test_agg_pow_stri32_f64_min_max();
  test_agg_pow_stri32_f64_min_min();
  test_agg_pow_stri32_f64_min_nan();
  test_agg_pow_stri32_f64_min_pinf();
  test_agg_pow_stri32_f64_min_zero();
  test_agg_pow_stri32_f64_min_ninf();
  test_agg_pow_stri32_i32_max_max();
  test_agg_pow_stri32_i32_max_zero();
  test_agg_pow_stri32_i32_max_normal();
  test_agg_pow_stri32_i32_max_min();
  test_agg_pow_stri32_i32_zero_max();
  test_agg_pow_stri32_i32_zero_zero();
  test_agg_pow_stri32_i32_zero_normal();
  test_agg_pow_stri32_i32_zero_min();
  test_agg_pow_stri32_i32_normal_max();
  test_agg_pow_stri32_i32_normal_zero();
  test_agg_pow_stri32_i32_normal_normal();
  test_agg_pow_stri32_i32_normal_min();
  test_agg_pow_stri32_i32_min_max();
  test_agg_pow_stri32_i32_min_zero();
  test_agg_pow_stri32_i32_min_normal();
  test_agg_pow_stri32_i32_min_min();
  test_agg_pow_stri32_stri32_max_max();
  test_agg_pow_stri32_stri32_max_zero();
  test_agg_pow_stri32_stri32_max_normal();
  test_agg_pow_stri32_stri32_max_min();
  test_agg_pow_stri32_stri32_zero_max();
  test_agg_pow_stri32_stri32_zero_zero();
  test_agg_pow_stri32_stri32_zero_normal();
  test_agg_pow_stri32_stri32_zero_min();
  test_agg_pow_stri32_stri32_normal_max();
  test_agg_pow_stri32_stri32_normal_zero();
  test_agg_pow_stri32_stri32_normal_normal();
  test_agg_pow_stri32_stri32_normal_min();
  test_agg_pow_stri32_stri32_min_max();
  test_agg_pow_stri32_stri32_min_zero();
  test_agg_pow_stri32_stri32_min_normal();
  test_agg_pow_stri32_stri32_min_min();
  test_agg_pow_stri32_strf64_max_max();
  test_agg_pow_stri32_strf64_max_zero();
  test_agg_pow_stri32_strf64_max_normal();
  test_agg_pow_stri32_strf64_max_min();
  test_agg_pow_stri32_strf64_zero_max();
  test_agg_pow_stri32_strf64_zero_zero();
  test_agg_pow_stri32_strf64_zero_normal();
  test_agg_pow_stri32_strf64_zero_min();
  test_agg_pow_stri32_strf64_normal_max();
  test_agg_pow_stri32_strf64_normal_zero();
  test_agg_pow_stri32_strf64_normal_normal();
  test_agg_pow_stri32_strf64_normal_min();
  test_agg_pow_stri32_strf64_min_max();
  test_agg_pow_stri32_strf64_min_zero();
  test_agg_pow_stri32_strf64_min_normal();
  test_agg_pow_stri32_strf64_min_min();
  test_agg_pow_stri32_bool_max_max();
  test_agg_pow_stri32_bool_max_zero();
  test_agg_pow_stri32_bool_max_normal();
  test_agg_pow_stri32_bool_max_min();
  test_agg_pow_stri32_bool_zero_max();
  test_agg_pow_stri32_bool_zero_zero();
  test_agg_pow_stri32_bool_zero_normal();
  test_agg_pow_stri32_bool_zero_min();
  test_agg_pow_stri32_bool_normal_max();
  test_agg_pow_stri32_bool_normal_zero();
  test_agg_pow_stri32_bool_normal_normal();
  test_agg_pow_stri32_bool_normal_min();
  test_agg_pow_stri32_bool_min_max();
  test_agg_pow_stri32_bool_min_zero();
  test_agg_pow_stri32_bool_min_normal();
  test_agg_pow_stri32_bool_min_min();
  test_agg_pow_stri32_object_max_max();
  test_agg_pow_stri32_object_max_zero();
  test_agg_pow_stri32_object_max_normal();
  test_agg_pow_stri32_object_max_min();
  test_agg_pow_stri32_object_zero_max();
  test_agg_pow_stri32_object_zero_zero();
  test_agg_pow_stri32_object_zero_normal();
  test_agg_pow_stri32_object_zero_min();
  test_agg_pow_stri32_object_normal_max();
  test_agg_pow_stri32_object_normal_zero();
  test_agg_pow_stri32_object_normal_normal();
  test_agg_pow_stri32_object_normal_min();
  test_agg_pow_stri32_object_min_max();
  test_agg_pow_stri32_object_min_zero();
  test_agg_pow_stri32_object_min_normal();
  test_agg_pow_stri32_object_min_min();
  test_agg_pow_strf64_f64_max_normal();
  test_agg_pow_strf64_f64_max_max();
  test_agg_pow_strf64_f64_max_min();
  test_agg_pow_strf64_f64_max_nan();
  test_agg_pow_strf64_f64_max_pinf();
  test_agg_pow_strf64_f64_max_zero();
  test_agg_pow_strf64_f64_max_ninf();
  test_agg_pow_strf64_f64_zero_normal();
  test_agg_pow_strf64_f64_zero_max();
  test_agg_pow_strf64_f64_zero_min();
  test_agg_pow_strf64_f64_zero_nan();
  test_agg_pow_strf64_f64_zero_pinf();
  test_agg_pow_strf64_f64_zero_zero();
  test_agg_pow_strf64_f64_zero_ninf();
  test_agg_pow_strf64_f64_normal_normal();
  test_agg_pow_strf64_f64_normal_max();
  test_agg_pow_strf64_f64_normal_min();
  test_agg_pow_strf64_f64_normal_nan();
  test_agg_pow_strf64_f64_normal_pinf();
  test_agg_pow_strf64_f64_normal_zero();
  test_agg_pow_strf64_f64_normal_ninf();
  test_agg_pow_strf64_f64_min_normal();
  test_agg_pow_strf64_f64_min_max();
  test_agg_pow_strf64_f64_min_min();
  test_agg_pow_strf64_f64_min_nan();
  test_agg_pow_strf64_f64_min_pinf();
  test_agg_pow_strf64_f64_min_zero();
  test_agg_pow_strf64_f64_min_ninf();
  test_agg_pow_strf64_i32_max_max();
  test_agg_pow_strf64_i32_max_zero();
  test_agg_pow_strf64_i32_max_normal();
  test_agg_pow_strf64_i32_max_min();
  test_agg_pow_strf64_i32_zero_max();
  test_agg_pow_strf64_i32_zero_zero();
  test_agg_pow_strf64_i32_zero_normal();
  test_agg_pow_strf64_i32_zero_min();
  test_agg_pow_strf64_i32_normal_max();
  test_agg_pow_strf64_i32_normal_zero();
  test_agg_pow_strf64_i32_normal_normal();
  test_agg_pow_strf64_i32_normal_min();
  test_agg_pow_strf64_i32_min_max();
  test_agg_pow_strf64_i32_min_zero();
  test_agg_pow_strf64_i32_min_normal();
  test_agg_pow_strf64_i32_min_min();
  test_agg_pow_strf64_stri32_max_max();
  test_agg_pow_strf64_stri32_max_zero();
  test_agg_pow_strf64_stri32_max_normal();
  test_agg_pow_strf64_stri32_max_min();
  test_agg_pow_strf64_stri32_zero_max();
  test_agg_pow_strf64_stri32_zero_zero();
  test_agg_pow_strf64_stri32_zero_normal();
  test_agg_pow_strf64_stri32_zero_min();
  test_agg_pow_strf64_stri32_normal_max();
  test_agg_pow_strf64_stri32_normal_zero();
  test_agg_pow_strf64_stri32_normal_normal();
  test_agg_pow_strf64_stri32_normal_min();
  test_agg_pow_strf64_stri32_min_max();
  test_agg_pow_strf64_stri32_min_zero();
  test_agg_pow_strf64_stri32_min_normal();
  test_agg_pow_strf64_stri32_min_min();
  test_agg_pow_strf64_strf64_max_max();
  test_agg_pow_strf64_strf64_max_zero();
  test_agg_pow_strf64_strf64_max_normal();
  test_agg_pow_strf64_strf64_max_min();
  test_agg_pow_strf64_strf64_zero_max();
  test_agg_pow_strf64_strf64_zero_zero();
  test_agg_pow_strf64_strf64_zero_normal();
  test_agg_pow_strf64_strf64_zero_min();
  test_agg_pow_strf64_strf64_normal_max();
  test_agg_pow_strf64_strf64_normal_zero();
  test_agg_pow_strf64_strf64_normal_normal();
  test_agg_pow_strf64_strf64_normal_min();
  test_agg_pow_strf64_strf64_min_max();
  test_agg_pow_strf64_strf64_min_zero();
  test_agg_pow_strf64_strf64_min_normal();
  test_agg_pow_strf64_strf64_min_min();
  test_agg_pow_strf64_bool_max_max();
  test_agg_pow_strf64_bool_max_zero();
  test_agg_pow_strf64_bool_max_normal();
  test_agg_pow_strf64_bool_max_min();
  test_agg_pow_strf64_bool_zero_max();
  test_agg_pow_strf64_bool_zero_zero();
  test_agg_pow_strf64_bool_zero_normal();
  test_agg_pow_strf64_bool_zero_min();
  test_agg_pow_strf64_bool_normal_max();
  test_agg_pow_strf64_bool_normal_zero();
  test_agg_pow_strf64_bool_normal_normal();
  test_agg_pow_strf64_bool_normal_min();
  test_agg_pow_strf64_bool_min_max();
  test_agg_pow_strf64_bool_min_zero();
  test_agg_pow_strf64_bool_min_normal();
  test_agg_pow_strf64_bool_min_min();
  test_agg_pow_strf64_object_max_max();
  test_agg_pow_strf64_object_max_zero();
  test_agg_pow_strf64_object_max_normal();
  test_agg_pow_strf64_object_max_min();
  test_agg_pow_strf64_object_zero_max();
  test_agg_pow_strf64_object_zero_zero();
  test_agg_pow_strf64_object_zero_normal();
  test_agg_pow_strf64_object_zero_min();
  test_agg_pow_strf64_object_normal_max();
  test_agg_pow_strf64_object_normal_zero();
  test_agg_pow_strf64_object_normal_normal();
  test_agg_pow_strf64_object_normal_min();
  test_agg_pow_strf64_object_min_max();
  test_agg_pow_strf64_object_min_zero();
  test_agg_pow_strf64_object_min_normal();
  test_agg_pow_strf64_object_min_min();
  test_agg_pow_bool_f64_max_normal();
  test_agg_pow_bool_f64_max_max();
  test_agg_pow_bool_f64_max_min();
  test_agg_pow_bool_f64_max_nan();
  test_agg_pow_bool_f64_max_pinf();
  test_agg_pow_bool_f64_max_zero();
  test_agg_pow_bool_f64_max_ninf();
  test_agg_pow_bool_f64_zero_normal();
  test_agg_pow_bool_f64_zero_max();
  test_agg_pow_bool_f64_zero_min();
  test_agg_pow_bool_f64_zero_nan();
  test_agg_pow_bool_f64_zero_pinf();
  test_agg_pow_bool_f64_zero_zero();
  test_agg_pow_bool_f64_zero_ninf();
  test_agg_pow_bool_f64_normal_normal();
  test_agg_pow_bool_f64_normal_max();
  test_agg_pow_bool_f64_normal_min();
  test_agg_pow_bool_f64_normal_nan();
  test_agg_pow_bool_f64_normal_pinf();
  test_agg_pow_bool_f64_normal_zero();
  test_agg_pow_bool_f64_normal_ninf();
  test_agg_pow_bool_f64_min_normal();
  test_agg_pow_bool_f64_min_max();
  test_agg_pow_bool_f64_min_min();
  test_agg_pow_bool_f64_min_nan();
  test_agg_pow_bool_f64_min_pinf();
  test_agg_pow_bool_f64_min_zero();
  test_agg_pow_bool_f64_min_ninf();
  test_agg_pow_bool_i32_max_max();
  test_agg_pow_bool_i32_max_zero();
  test_agg_pow_bool_i32_max_normal();
  test_agg_pow_bool_i32_max_min();
  test_agg_pow_bool_i32_zero_max();
  test_agg_pow_bool_i32_zero_zero();
  test_agg_pow_bool_i32_zero_normal();
  test_agg_pow_bool_i32_zero_min();
  test_agg_pow_bool_i32_normal_max();
  test_agg_pow_bool_i32_normal_zero();
  test_agg_pow_bool_i32_normal_normal();
  test_agg_pow_bool_i32_normal_min();
  test_agg_pow_bool_i32_min_max();
  test_agg_pow_bool_i32_min_zero();
  test_agg_pow_bool_i32_min_normal();
  test_agg_pow_bool_i32_min_min();
  test_agg_pow_bool_stri32_max_max();
  test_agg_pow_bool_stri32_max_zero();
  test_agg_pow_bool_stri32_max_normal();
  test_agg_pow_bool_stri32_max_min();
  test_agg_pow_bool_stri32_zero_max();
  test_agg_pow_bool_stri32_zero_zero();
  test_agg_pow_bool_stri32_zero_normal();
  test_agg_pow_bool_stri32_zero_min();
  test_agg_pow_bool_stri32_normal_max();
  test_agg_pow_bool_stri32_normal_zero();
  test_agg_pow_bool_stri32_normal_normal();
  test_agg_pow_bool_stri32_normal_min();
  test_agg_pow_bool_stri32_min_max();
  test_agg_pow_bool_stri32_min_zero();
  test_agg_pow_bool_stri32_min_normal();
  test_agg_pow_bool_stri32_min_min();
  test_agg_pow_bool_strf64_max_max();
  test_agg_pow_bool_strf64_max_zero();
  test_agg_pow_bool_strf64_max_normal();
  test_agg_pow_bool_strf64_max_min();
  test_agg_pow_bool_strf64_zero_max();
  test_agg_pow_bool_strf64_zero_zero();
  test_agg_pow_bool_strf64_zero_normal();
  test_agg_pow_bool_strf64_zero_min();
  test_agg_pow_bool_strf64_normal_max();
  test_agg_pow_bool_strf64_normal_zero();
  test_agg_pow_bool_strf64_normal_normal();
  test_agg_pow_bool_strf64_normal_min();
  test_agg_pow_bool_strf64_min_max();
  test_agg_pow_bool_strf64_min_zero();
  test_agg_pow_bool_strf64_min_normal();
  test_agg_pow_bool_strf64_min_min();
  test_agg_pow_bool_bool_max_max();
  test_agg_pow_bool_bool_max_zero();
  test_agg_pow_bool_bool_max_normal();
  test_agg_pow_bool_bool_max_min();
  test_agg_pow_bool_bool_zero_max();
  test_agg_pow_bool_bool_zero_zero();
  test_agg_pow_bool_bool_zero_normal();
  test_agg_pow_bool_bool_zero_min();
  test_agg_pow_bool_bool_normal_max();
  test_agg_pow_bool_bool_normal_zero();
  test_agg_pow_bool_bool_normal_normal();
  test_agg_pow_bool_bool_normal_min();
  test_agg_pow_bool_bool_min_max();
  test_agg_pow_bool_bool_min_zero();
  test_agg_pow_bool_bool_min_normal();
  test_agg_pow_bool_bool_min_min();
  test_agg_pow_bool_object_max_max();
  test_agg_pow_bool_object_max_zero();
  test_agg_pow_bool_object_max_normal();
  test_agg_pow_bool_object_max_min();
  test_agg_pow_bool_object_zero_max();
  test_agg_pow_bool_object_zero_zero();
  test_agg_pow_bool_object_zero_normal();
  test_agg_pow_bool_object_zero_min();
  test_agg_pow_bool_object_normal_max();
  test_agg_pow_bool_object_normal_zero();
  test_agg_pow_bool_object_normal_normal();
  test_agg_pow_bool_object_normal_min();
  test_agg_pow_bool_object_min_max();
  test_agg_pow_bool_object_min_zero();
  test_agg_pow_bool_object_min_normal();
  test_agg_pow_bool_object_min_min();
  test_agg_pow_object_f64_max_normal();
  test_agg_pow_object_f64_max_max();
  test_agg_pow_object_f64_max_min();
  test_agg_pow_object_f64_max_nan();
  test_agg_pow_object_f64_max_pinf();
  test_agg_pow_object_f64_max_zero();
  test_agg_pow_object_f64_max_ninf();
  test_agg_pow_object_f64_zero_normal();
  test_agg_pow_object_f64_zero_max();
  test_agg_pow_object_f64_zero_min();
  test_agg_pow_object_f64_zero_nan();
  test_agg_pow_object_f64_zero_pinf();
  test_agg_pow_object_f64_zero_zero();
  test_agg_pow_object_f64_zero_ninf();
  test_agg_pow_object_f64_normal_normal();
  test_agg_pow_object_f64_normal_max();
  test_agg_pow_object_f64_normal_min();
  test_agg_pow_object_f64_normal_nan();
  test_agg_pow_object_f64_normal_pinf();
  test_agg_pow_object_f64_normal_zero();
  test_agg_pow_object_f64_normal_ninf();
  test_agg_pow_object_f64_min_normal();
  test_agg_pow_object_f64_min_max();
  test_agg_pow_object_f64_min_min();
  test_agg_pow_object_f64_min_nan();
  test_agg_pow_object_f64_min_pinf();
  test_agg_pow_object_f64_min_zero();
  test_agg_pow_object_f64_min_ninf();
  test_agg_pow_object_i32_max_max();
  test_agg_pow_object_i32_max_zero();
  test_agg_pow_object_i32_max_normal();
  test_agg_pow_object_i32_max_min();
  test_agg_pow_object_i32_zero_max();
  test_agg_pow_object_i32_zero_zero();
  test_agg_pow_object_i32_zero_normal();
  test_agg_pow_object_i32_zero_min();
  test_agg_pow_object_i32_normal_max();
  test_agg_pow_object_i32_normal_zero();
  test_agg_pow_object_i32_normal_normal();
  test_agg_pow_object_i32_normal_min();
  test_agg_pow_object_i32_min_max();
  test_agg_pow_object_i32_min_zero();
  test_agg_pow_object_i32_min_normal();
  test_agg_pow_object_i32_min_min();
  test_agg_pow_object_stri32_max_max();
  test_agg_pow_object_stri32_max_zero();
  test_agg_pow_object_stri32_max_normal();
  test_agg_pow_object_stri32_max_min();
  test_agg_pow_object_stri32_zero_max();
  test_agg_pow_object_stri32_zero_zero();
  test_agg_pow_object_stri32_zero_normal();
  test_agg_pow_object_stri32_zero_min();
  test_agg_pow_object_stri32_normal_max();
  test_agg_pow_object_stri32_normal_zero();
  test_agg_pow_object_stri32_normal_normal();
  test_agg_pow_object_stri32_normal_min();
  test_agg_pow_object_stri32_min_max();
  test_agg_pow_object_stri32_min_zero();
  test_agg_pow_object_stri32_min_normal();
  test_agg_pow_object_stri32_min_min();
  test_agg_pow_object_strf64_max_max();
  test_agg_pow_object_strf64_max_zero();
  test_agg_pow_object_strf64_max_normal();
  test_agg_pow_object_strf64_max_min();
  test_agg_pow_object_strf64_zero_max();
  test_agg_pow_object_strf64_zero_zero();
  test_agg_pow_object_strf64_zero_normal();
  test_agg_pow_object_strf64_zero_min();
  test_agg_pow_object_strf64_normal_max();
  test_agg_pow_object_strf64_normal_zero();
  test_agg_pow_object_strf64_normal_normal();
  test_agg_pow_object_strf64_normal_min();
  test_agg_pow_object_strf64_min_max();
  test_agg_pow_object_strf64_min_zero();
  test_agg_pow_object_strf64_min_normal();
  test_agg_pow_object_strf64_min_min();
  test_agg_pow_object_bool_max_max();
  test_agg_pow_object_bool_max_zero();
  test_agg_pow_object_bool_max_normal();
  test_agg_pow_object_bool_max_min();
  test_agg_pow_object_bool_zero_max();
  test_agg_pow_object_bool_zero_zero();
  test_agg_pow_object_bool_zero_normal();
  test_agg_pow_object_bool_zero_min();
  test_agg_pow_object_bool_normal_max();
  test_agg_pow_object_bool_normal_zero();
  test_agg_pow_object_bool_normal_normal();
  test_agg_pow_object_bool_normal_min();
  test_agg_pow_object_bool_min_max();
  test_agg_pow_object_bool_min_zero();
  test_agg_pow_object_bool_min_normal();
  test_agg_pow_object_bool_min_min();
  test_agg_pow_object_object_max_max();
  test_agg_pow_object_object_max_zero();
  test_agg_pow_object_object_max_normal();
  test_agg_pow_object_object_max_min();
  test_agg_pow_object_object_zero_max();
  test_agg_pow_object_object_zero_zero();
  test_agg_pow_object_object_zero_normal();
  test_agg_pow_object_object_zero_min();
  test_agg_pow_object_object_normal_max();
  test_agg_pow_object_object_normal_zero();
  test_agg_pow_object_object_normal_normal();
  test_agg_pow_object_object_normal_min();
  test_agg_pow_object_object_min_max();
  test_agg_pow_object_object_min_zero();
  test_agg_pow_object_object_min_normal();
  test_agg_pow_object_object_min_min();

}
// ----------------------------------------------------------------