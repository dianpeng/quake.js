// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_shl_f64_f64_normal_normal() {
  dump("testing: test_shl_f64_f64_normal_normal");
  let a0 = 13.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_max() {
  dump("testing: test_shl_f64_f64_normal_max");
  let a0 = 13.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_min() {
  dump("testing: test_shl_f64_f64_normal_min");
  let a0 = 13.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_nan() {
  dump("testing: test_shl_f64_f64_normal_nan");
  let a0 = 13.0;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_pinf() {
  dump("testing: test_shl_f64_f64_normal_pinf");
  let a0 = 13.0;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_zero() {
  dump("testing: test_shl_f64_f64_normal_zero");
  let a0 = 13.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_normal_ninf() {
  dump("testing: test_shl_f64_f64_normal_ninf");
  let a0 = 13.0;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_normal() {
  dump("testing: test_shl_f64_f64_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_max() {
  dump("testing: test_shl_f64_f64_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_min() {
  dump("testing: test_shl_f64_f64_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_nan() {
  dump("testing: test_shl_f64_f64_max_nan");
  let a0 = 2.22507385851e-308;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_pinf() {
  dump("testing: test_shl_f64_f64_max_pinf");
  let a0 = 2.22507385851e-308;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_zero() {
  dump("testing: test_shl_f64_f64_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_max_ninf() {
  dump("testing: test_shl_f64_f64_max_ninf");
  let a0 = 2.22507385851e-308;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_normal() {
  dump("testing: test_shl_f64_f64_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_max() {
  dump("testing: test_shl_f64_f64_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_min() {
  dump("testing: test_shl_f64_f64_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_nan() {
  dump("testing: test_shl_f64_f64_min_nan");
  let a0 = 1.79769313486e+308;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_pinf() {
  dump("testing: test_shl_f64_f64_min_pinf");
  let a0 = 1.79769313486e+308;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_zero() {
  dump("testing: test_shl_f64_f64_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_min_ninf() {
  dump("testing: test_shl_f64_f64_min_ninf");
  let a0 = 1.79769313486e+308;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_normal() {
  dump("testing: test_shl_f64_f64_nan_normal");
  let a0 = NaN;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_max() {
  dump("testing: test_shl_f64_f64_nan_max");
  let a0 = NaN;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_min() {
  dump("testing: test_shl_f64_f64_nan_min");
  let a0 = NaN;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_nan() {
  dump("testing: test_shl_f64_f64_nan_nan");
  let a0 = NaN;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_pinf() {
  dump("testing: test_shl_f64_f64_nan_pinf");
  let a0 = NaN;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_zero() {
  dump("testing: test_shl_f64_f64_nan_zero");
  let a0 = NaN;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_nan_ninf() {
  dump("testing: test_shl_f64_f64_nan_ninf");
  let a0 = NaN;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_normal() {
  dump("testing: test_shl_f64_f64_pinf_normal");
  let a0 = Infinity;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_max() {
  dump("testing: test_shl_f64_f64_pinf_max");
  let a0 = Infinity;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_min() {
  dump("testing: test_shl_f64_f64_pinf_min");
  let a0 = Infinity;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_nan() {
  dump("testing: test_shl_f64_f64_pinf_nan");
  let a0 = Infinity;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_pinf() {
  dump("testing: test_shl_f64_f64_pinf_pinf");
  let a0 = Infinity;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_zero() {
  dump("testing: test_shl_f64_f64_pinf_zero");
  let a0 = Infinity;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_pinf_ninf() {
  dump("testing: test_shl_f64_f64_pinf_ninf");
  let a0 = Infinity;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_normal() {
  dump("testing: test_shl_f64_f64_zero_normal");
  let a0 = 0.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_max() {
  dump("testing: test_shl_f64_f64_zero_max");
  let a0 = 0.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_min() {
  dump("testing: test_shl_f64_f64_zero_min");
  let a0 = 0.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_nan() {
  dump("testing: test_shl_f64_f64_zero_nan");
  let a0 = 0.0;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_pinf() {
  dump("testing: test_shl_f64_f64_zero_pinf");
  let a0 = 0.0;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_zero() {
  dump("testing: test_shl_f64_f64_zero_zero");
  let a0 = 0.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_zero_ninf() {
  dump("testing: test_shl_f64_f64_zero_ninf");
  let a0 = 0.0;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_normal() {
  dump("testing: test_shl_f64_f64_ninf_normal");
  let a0 = -Infinity;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_max() {
  dump("testing: test_shl_f64_f64_ninf_max");
  let a0 = -Infinity;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_min() {
  dump("testing: test_shl_f64_f64_ninf_min");
  let a0 = -Infinity;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_nan() {
  dump("testing: test_shl_f64_f64_ninf_nan");
  let a0 = -Infinity;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_pinf() {
  dump("testing: test_shl_f64_f64_ninf_pinf");
  let a0 = -Infinity;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_zero() {
  dump("testing: test_shl_f64_f64_ninf_zero");
  let a0 = -Infinity;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_f64_ninf_ninf() {
  dump("testing: test_shl_f64_f64_ninf_ninf");
  let a0 = -Infinity;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_normal_max() {
  dump("testing: test_shl_f64_i32_normal_max");
  let a0 = 13.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_normal_zero() {
  dump("testing: test_shl_f64_i32_normal_zero");
  let a0 = 13.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_normal_normal() {
  dump("testing: test_shl_f64_i32_normal_normal");
  let a0 = 13.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_normal_min() {
  dump("testing: test_shl_f64_i32_normal_min");
  let a0 = 13.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_max_max() {
  dump("testing: test_shl_f64_i32_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_max_zero() {
  dump("testing: test_shl_f64_i32_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_max_normal() {
  dump("testing: test_shl_f64_i32_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_max_min() {
  dump("testing: test_shl_f64_i32_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_min_max() {
  dump("testing: test_shl_f64_i32_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_min_zero() {
  dump("testing: test_shl_f64_i32_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_min_normal() {
  dump("testing: test_shl_f64_i32_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_min_min() {
  dump("testing: test_shl_f64_i32_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_nan_max() {
  dump("testing: test_shl_f64_i32_nan_max");
  let a0 = NaN;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_nan_zero() {
  dump("testing: test_shl_f64_i32_nan_zero");
  let a0 = NaN;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_nan_normal() {
  dump("testing: test_shl_f64_i32_nan_normal");
  let a0 = NaN;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_nan_min() {
  dump("testing: test_shl_f64_i32_nan_min");
  let a0 = NaN;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_pinf_max() {
  dump("testing: test_shl_f64_i32_pinf_max");
  let a0 = Infinity;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_pinf_zero() {
  dump("testing: test_shl_f64_i32_pinf_zero");
  let a0 = Infinity;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_pinf_normal() {
  dump("testing: test_shl_f64_i32_pinf_normal");
  let a0 = Infinity;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_pinf_min() {
  dump("testing: test_shl_f64_i32_pinf_min");
  let a0 = Infinity;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_zero_max() {
  dump("testing: test_shl_f64_i32_zero_max");
  let a0 = 0.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_zero_zero() {
  dump("testing: test_shl_f64_i32_zero_zero");
  let a0 = 0.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_zero_normal() {
  dump("testing: test_shl_f64_i32_zero_normal");
  let a0 = 0.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_zero_min() {
  dump("testing: test_shl_f64_i32_zero_min");
  let a0 = 0.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_ninf_max() {
  dump("testing: test_shl_f64_i32_ninf_max");
  let a0 = -Infinity;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_ninf_zero() {
  dump("testing: test_shl_f64_i32_ninf_zero");
  let a0 = -Infinity;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_ninf_normal() {
  dump("testing: test_shl_f64_i32_ninf_normal");
  let a0 = -Infinity;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_f64_i32_ninf_min() {
  dump("testing: test_shl_f64_i32_ninf_min");
  let a0 = -Infinity;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_normal() {
  dump("testing: test_shl_i32_f64_max_normal");
  let a0 = 2147483647;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_max() {
  dump("testing: test_shl_i32_f64_max_max");
  let a0 = 2147483647;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_min() {
  dump("testing: test_shl_i32_f64_max_min");
  let a0 = 2147483647;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_nan() {
  dump("testing: test_shl_i32_f64_max_nan");
  let a0 = 2147483647;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_pinf() {
  dump("testing: test_shl_i32_f64_max_pinf");
  let a0 = 2147483647;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_zero() {
  dump("testing: test_shl_i32_f64_max_zero");
  let a0 = 2147483647;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_max_ninf() {
  dump("testing: test_shl_i32_f64_max_ninf");
  let a0 = 2147483647;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_normal() {
  dump("testing: test_shl_i32_f64_zero_normal");
  let a0 = 0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_max() {
  dump("testing: test_shl_i32_f64_zero_max");
  let a0 = 0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_min() {
  dump("testing: test_shl_i32_f64_zero_min");
  let a0 = 0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_nan() {
  dump("testing: test_shl_i32_f64_zero_nan");
  let a0 = 0;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_pinf() {
  dump("testing: test_shl_i32_f64_zero_pinf");
  let a0 = 0;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_zero() {
  dump("testing: test_shl_i32_f64_zero_zero");
  let a0 = 0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_zero_ninf() {
  dump("testing: test_shl_i32_f64_zero_ninf");
  let a0 = 0;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_normal() {
  dump("testing: test_shl_i32_f64_normal_normal");
  let a0 = 14;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_max() {
  dump("testing: test_shl_i32_f64_normal_max");
  let a0 = 14;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_min() {
  dump("testing: test_shl_i32_f64_normal_min");
  let a0 = 14;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_nan() {
  dump("testing: test_shl_i32_f64_normal_nan");
  let a0 = 14;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_pinf() {
  dump("testing: test_shl_i32_f64_normal_pinf");
  let a0 = 14;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_zero() {
  dump("testing: test_shl_i32_f64_normal_zero");
  let a0 = 14;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_normal_ninf() {
  dump("testing: test_shl_i32_f64_normal_ninf");
  let a0 = 14;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_normal() {
  dump("testing: test_shl_i32_f64_min_normal");
  let a0 = -2147483648;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_max() {
  dump("testing: test_shl_i32_f64_min_max");
  let a0 = -2147483648;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_min() {
  dump("testing: test_shl_i32_f64_min_min");
  let a0 = -2147483648;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_nan() {
  dump("testing: test_shl_i32_f64_min_nan");
  let a0 = -2147483648;
  let a1 = NaN;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_pinf() {
  dump("testing: test_shl_i32_f64_min_pinf");
  let a0 = -2147483648;
  let a1 = Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_zero() {
  dump("testing: test_shl_i32_f64_min_zero");
  let a0 = -2147483648;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_f64_min_ninf() {
  dump("testing: test_shl_i32_f64_min_ninf");
  let a0 = -2147483648;
  let a1 = -Infinity;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_max_max() {
  dump("testing: test_shl_i32_i32_max_max");
  let a0 = 2147483647;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_max_zero() {
  dump("testing: test_shl_i32_i32_max_zero");
  let a0 = 2147483647;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_max_normal() {
  dump("testing: test_shl_i32_i32_max_normal");
  let a0 = 2147483647;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_max_min() {
  dump("testing: test_shl_i32_i32_max_min");
  let a0 = 2147483647;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_zero_max() {
  dump("testing: test_shl_i32_i32_zero_max");
  let a0 = 0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_zero_zero() {
  dump("testing: test_shl_i32_i32_zero_zero");
  let a0 = 0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_zero_normal() {
  dump("testing: test_shl_i32_i32_zero_normal");
  let a0 = 0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_zero_min() {
  dump("testing: test_shl_i32_i32_zero_min");
  let a0 = 0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_normal_max() {
  dump("testing: test_shl_i32_i32_normal_max");
  let a0 = 14;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_normal_zero() {
  dump("testing: test_shl_i32_i32_normal_zero");
  let a0 = 14;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_normal_normal() {
  dump("testing: test_shl_i32_i32_normal_normal");
  let a0 = 14;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_normal_min() {
  dump("testing: test_shl_i32_i32_normal_min");
  let a0 = 14;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_min_max() {
  dump("testing: test_shl_i32_i32_min_max");
  let a0 = -2147483648;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_min_zero() {
  dump("testing: test_shl_i32_i32_min_zero");
  let a0 = -2147483648;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_min_normal() {
  dump("testing: test_shl_i32_i32_min_normal");
  let a0 = -2147483648;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}


function test_shl_i32_i32_min_min() {
  dump("testing: test_shl_i32_i32_min_min");
  let a0 = -2147483648;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 << a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 << a1;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}
  test_shl_f64_f64_normal_normal();
  test_shl_f64_f64_normal_max();
  test_shl_f64_f64_normal_min();
  test_shl_f64_f64_normal_nan();
  test_shl_f64_f64_normal_pinf();
  test_shl_f64_f64_normal_zero();
  test_shl_f64_f64_normal_ninf();
  test_shl_f64_f64_max_normal();
  test_shl_f64_f64_max_max();
  test_shl_f64_f64_max_min();
  test_shl_f64_f64_max_nan();
  test_shl_f64_f64_max_pinf();
  test_shl_f64_f64_max_zero();
  test_shl_f64_f64_max_ninf();
  test_shl_f64_f64_min_normal();
  test_shl_f64_f64_min_max();
  test_shl_f64_f64_min_min();
  test_shl_f64_f64_min_nan();
  test_shl_f64_f64_min_pinf();
  test_shl_f64_f64_min_zero();
  test_shl_f64_f64_min_ninf();
  test_shl_f64_f64_nan_normal();
  test_shl_f64_f64_nan_max();
  test_shl_f64_f64_nan_min();
  test_shl_f64_f64_nan_nan();
  test_shl_f64_f64_nan_pinf();
  test_shl_f64_f64_nan_zero();
  test_shl_f64_f64_nan_ninf();
  test_shl_f64_f64_pinf_normal();
  test_shl_f64_f64_pinf_max();
  test_shl_f64_f64_pinf_min();
  test_shl_f64_f64_pinf_nan();
  test_shl_f64_f64_pinf_pinf();
  test_shl_f64_f64_pinf_zero();
  test_shl_f64_f64_pinf_ninf();
  test_shl_f64_f64_zero_normal();
  test_shl_f64_f64_zero_max();
  test_shl_f64_f64_zero_min();
  test_shl_f64_f64_zero_nan();
  test_shl_f64_f64_zero_pinf();
  test_shl_f64_f64_zero_zero();
  test_shl_f64_f64_zero_ninf();
  test_shl_f64_f64_ninf_normal();
  test_shl_f64_f64_ninf_max();
  test_shl_f64_f64_ninf_min();
  test_shl_f64_f64_ninf_nan();
  test_shl_f64_f64_ninf_pinf();
  test_shl_f64_f64_ninf_zero();
  test_shl_f64_f64_ninf_ninf();
  test_shl_f64_i32_normal_max();
  test_shl_f64_i32_normal_zero();
  test_shl_f64_i32_normal_normal();
  test_shl_f64_i32_normal_min();
  test_shl_f64_i32_max_max();
  test_shl_f64_i32_max_zero();
  test_shl_f64_i32_max_normal();
  test_shl_f64_i32_max_min();
  test_shl_f64_i32_min_max();
  test_shl_f64_i32_min_zero();
  test_shl_f64_i32_min_normal();
  test_shl_f64_i32_min_min();
  test_shl_f64_i32_nan_max();
  test_shl_f64_i32_nan_zero();
  test_shl_f64_i32_nan_normal();
  test_shl_f64_i32_nan_min();
  test_shl_f64_i32_pinf_max();
  test_shl_f64_i32_pinf_zero();
  test_shl_f64_i32_pinf_normal();
  test_shl_f64_i32_pinf_min();
  test_shl_f64_i32_zero_max();
  test_shl_f64_i32_zero_zero();
  test_shl_f64_i32_zero_normal();
  test_shl_f64_i32_zero_min();
  test_shl_f64_i32_ninf_max();
  test_shl_f64_i32_ninf_zero();
  test_shl_f64_i32_ninf_normal();
  test_shl_f64_i32_ninf_min();
  test_shl_i32_f64_max_normal();
  test_shl_i32_f64_max_max();
  test_shl_i32_f64_max_min();
  test_shl_i32_f64_max_nan();
  test_shl_i32_f64_max_pinf();
  test_shl_i32_f64_max_zero();
  test_shl_i32_f64_max_ninf();
  test_shl_i32_f64_zero_normal();
  test_shl_i32_f64_zero_max();
  test_shl_i32_f64_zero_min();
  test_shl_i32_f64_zero_nan();
  test_shl_i32_f64_zero_pinf();
  test_shl_i32_f64_zero_zero();
  test_shl_i32_f64_zero_ninf();
  test_shl_i32_f64_normal_normal();
  test_shl_i32_f64_normal_max();
  test_shl_i32_f64_normal_min();
  test_shl_i32_f64_normal_nan();
  test_shl_i32_f64_normal_pinf();
  test_shl_i32_f64_normal_zero();
  test_shl_i32_f64_normal_ninf();
  test_shl_i32_f64_min_normal();
  test_shl_i32_f64_min_max();
  test_shl_i32_f64_min_min();
  test_shl_i32_f64_min_nan();
  test_shl_i32_f64_min_pinf();
  test_shl_i32_f64_min_zero();
  test_shl_i32_f64_min_ninf();
  test_shl_i32_i32_max_max();
  test_shl_i32_i32_max_zero();
  test_shl_i32_i32_max_normal();
  test_shl_i32_i32_max_min();
  test_shl_i32_i32_zero_max();
  test_shl_i32_i32_zero_zero();
  test_shl_i32_i32_zero_normal();
  test_shl_i32_i32_zero_min();
  test_shl_i32_i32_normal_max();
  test_shl_i32_i32_normal_zero();
  test_shl_i32_i32_normal_normal();
  test_shl_i32_i32_normal_min();
  test_shl_i32_i32_min_max();
  test_shl_i32_i32_min_zero();
  test_shl_i32_i32_min_normal();
  test_shl_i32_i32_min_min();

}
// ----------------------------------------------------------------