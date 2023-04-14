// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_shr_f64_f64_max_max() {
  dump("testing: test_shr_f64_f64_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_max_zero() {
  dump("testing: test_shr_f64_f64_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_max_normal() {
  dump("testing: test_shr_f64_f64_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_max_min() {
  dump("testing: test_shr_f64_f64_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_zero_max() {
  dump("testing: test_shr_f64_f64_zero_max");
  let a0 = 0.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_zero_zero() {
  dump("testing: test_shr_f64_f64_zero_zero");
  let a0 = 0.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_zero_normal() {
  dump("testing: test_shr_f64_f64_zero_normal");
  let a0 = 0.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_zero_min() {
  dump("testing: test_shr_f64_f64_zero_min");
  let a0 = 0.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_normal_max() {
  dump("testing: test_shr_f64_f64_normal_max");
  let a0 = 13.0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_normal_zero() {
  dump("testing: test_shr_f64_f64_normal_zero");
  let a0 = 13.0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_normal_normal() {
  dump("testing: test_shr_f64_f64_normal_normal");
  let a0 = 13.0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_normal_min() {
  dump("testing: test_shr_f64_f64_normal_min");
  let a0 = 13.0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_min_max() {
  dump("testing: test_shr_f64_f64_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_min_zero() {
  dump("testing: test_shr_f64_f64_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_min_normal() {
  dump("testing: test_shr_f64_f64_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_f64_min_min() {
  dump("testing: test_shr_f64_f64_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_max_max() {
  dump("testing: test_shr_f64_i32_max_max");
  let a0 = 2.22507385851e-308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_max_zero() {
  dump("testing: test_shr_f64_i32_max_zero");
  let a0 = 2.22507385851e-308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_max_normal() {
  dump("testing: test_shr_f64_i32_max_normal");
  let a0 = 2.22507385851e-308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_max_min() {
  dump("testing: test_shr_f64_i32_max_min");
  let a0 = 2.22507385851e-308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_zero_max() {
  dump("testing: test_shr_f64_i32_zero_max");
  let a0 = 0.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_zero_zero() {
  dump("testing: test_shr_f64_i32_zero_zero");
  let a0 = 0.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_zero_normal() {
  dump("testing: test_shr_f64_i32_zero_normal");
  let a0 = 0.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_zero_min() {
  dump("testing: test_shr_f64_i32_zero_min");
  let a0 = 0.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_normal_max() {
  dump("testing: test_shr_f64_i32_normal_max");
  let a0 = 13.0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_normal_zero() {
  dump("testing: test_shr_f64_i32_normal_zero");
  let a0 = 13.0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_normal_normal() {
  dump("testing: test_shr_f64_i32_normal_normal");
  let a0 = 13.0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_normal_min() {
  dump("testing: test_shr_f64_i32_normal_min");
  let a0 = 13.0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_min_max() {
  dump("testing: test_shr_f64_i32_min_max");
  let a0 = 1.79769313486e+308;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_min_zero() {
  dump("testing: test_shr_f64_i32_min_zero");
  let a0 = 1.79769313486e+308;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_min_normal() {
  dump("testing: test_shr_f64_i32_min_normal");
  let a0 = 1.79769313486e+308;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_f64_i32_min_min() {
  dump("testing: test_shr_f64_i32_min_min");
  let a0 = 1.79769313486e+308;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_max_max() {
  dump("testing: test_shr_i32_f64_max_max");
  let a0 = 2147483647;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_max_zero() {
  dump("testing: test_shr_i32_f64_max_zero");
  let a0 = 2147483647;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_max_normal() {
  dump("testing: test_shr_i32_f64_max_normal");
  let a0 = 2147483647;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_max_min() {
  dump("testing: test_shr_i32_f64_max_min");
  let a0 = 2147483647;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_zero_max() {
  dump("testing: test_shr_i32_f64_zero_max");
  let a0 = 0;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_zero_zero() {
  dump("testing: test_shr_i32_f64_zero_zero");
  let a0 = 0;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_zero_normal() {
  dump("testing: test_shr_i32_f64_zero_normal");
  let a0 = 0;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_zero_min() {
  dump("testing: test_shr_i32_f64_zero_min");
  let a0 = 0;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_normal_max() {
  dump("testing: test_shr_i32_f64_normal_max");
  let a0 = 14;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_normal_zero() {
  dump("testing: test_shr_i32_f64_normal_zero");
  let a0 = 14;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_normal_normal() {
  dump("testing: test_shr_i32_f64_normal_normal");
  let a0 = 14;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_normal_min() {
  dump("testing: test_shr_i32_f64_normal_min");
  let a0 = 14;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_min_max() {
  dump("testing: test_shr_i32_f64_min_max");
  let a0 = -2147483648;
  let a1 = 2.22507385851e-308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_min_zero() {
  dump("testing: test_shr_i32_f64_min_zero");
  let a0 = -2147483648;
  let a1 = 0.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_min_normal() {
  dump("testing: test_shr_i32_f64_min_normal");
  let a0 = -2147483648;
  let a1 = 13.0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_f64_min_min() {
  dump("testing: test_shr_i32_f64_min_min");
  let a0 = -2147483648;
  let a1 = 1.79769313486e+308;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_max_max() {
  dump("testing: test_shr_i32_i32_max_max");
  let a0 = 2147483647;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_max_zero() {
  dump("testing: test_shr_i32_i32_max_zero");
  let a0 = 2147483647;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_max_normal() {
  dump("testing: test_shr_i32_i32_max_normal");
  let a0 = 2147483647;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_max_min() {
  dump("testing: test_shr_i32_i32_max_min");
  let a0 = 2147483647;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_zero_max() {
  dump("testing: test_shr_i32_i32_zero_max");
  let a0 = 0;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_zero_zero() {
  dump("testing: test_shr_i32_i32_zero_zero");
  let a0 = 0;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_zero_normal() {
  dump("testing: test_shr_i32_i32_zero_normal");
  let a0 = 0;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_zero_min() {
  dump("testing: test_shr_i32_i32_zero_min");
  let a0 = 0;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_normal_max() {
  dump("testing: test_shr_i32_i32_normal_max");
  let a0 = 14;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_normal_zero() {
  dump("testing: test_shr_i32_i32_normal_zero");
  let a0 = 14;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_normal_normal() {
  dump("testing: test_shr_i32_i32_normal_normal");
  let a0 = 14;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_normal_min() {
  dump("testing: test_shr_i32_i32_normal_min");
  let a0 = 14;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_min_max() {
  dump("testing: test_shr_i32_i32_min_max");
  let a0 = -2147483648;
  let a1 = 2147483647;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_min_zero() {
  dump("testing: test_shr_i32_i32_min_zero");
  let a0 = -2147483648;
  let a1 = 0;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_min_normal() {
  dump("testing: test_shr_i32_i32_min_normal");
  let a0 = -2147483648;
  let a1 = 14;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}


function test_shr_i32_i32_min_min() {
  dump("testing: test_shr_i32_i32_min_min");
  let a0 = -2147483648;
  let a1 = -2147483648;
  let test_value = undefined;
  let expect = a0 >> a1;
  for (let i = 0; i < 50; i++) {
    test_value = a0 >> a1;
  }
  yes(test_value === expect);
}
  test_shr_f64_f64_max_max();
  test_shr_f64_f64_max_zero();
  test_shr_f64_f64_max_normal();
  test_shr_f64_f64_max_min();
  test_shr_f64_f64_zero_max();
  test_shr_f64_f64_zero_zero();
  test_shr_f64_f64_zero_normal();
  test_shr_f64_f64_zero_min();
  test_shr_f64_f64_normal_max();
  test_shr_f64_f64_normal_zero();
  test_shr_f64_f64_normal_normal();
  test_shr_f64_f64_normal_min();
  test_shr_f64_f64_min_max();
  test_shr_f64_f64_min_zero();
  test_shr_f64_f64_min_normal();
  test_shr_f64_f64_min_min();
  test_shr_f64_i32_max_max();
  test_shr_f64_i32_max_zero();
  test_shr_f64_i32_max_normal();
  test_shr_f64_i32_max_min();
  test_shr_f64_i32_zero_max();
  test_shr_f64_i32_zero_zero();
  test_shr_f64_i32_zero_normal();
  test_shr_f64_i32_zero_min();
  test_shr_f64_i32_normal_max();
  test_shr_f64_i32_normal_zero();
  test_shr_f64_i32_normal_normal();
  test_shr_f64_i32_normal_min();
  test_shr_f64_i32_min_max();
  test_shr_f64_i32_min_zero();
  test_shr_f64_i32_min_normal();
  test_shr_f64_i32_min_min();
  test_shr_i32_f64_max_max();
  test_shr_i32_f64_max_zero();
  test_shr_i32_f64_max_normal();
  test_shr_i32_f64_max_min();
  test_shr_i32_f64_zero_max();
  test_shr_i32_f64_zero_zero();
  test_shr_i32_f64_zero_normal();
  test_shr_i32_f64_zero_min();
  test_shr_i32_f64_normal_max();
  test_shr_i32_f64_normal_zero();
  test_shr_i32_f64_normal_normal();
  test_shr_i32_f64_normal_min();
  test_shr_i32_f64_min_max();
  test_shr_i32_f64_min_zero();
  test_shr_i32_f64_min_normal();
  test_shr_i32_f64_min_min();
  test_shr_i32_i32_max_max();
  test_shr_i32_i32_max_zero();
  test_shr_i32_i32_max_normal();
  test_shr_i32_i32_max_min();
  test_shr_i32_i32_zero_max();
  test_shr_i32_i32_zero_zero();
  test_shr_i32_i32_zero_normal();
  test_shr_i32_i32_zero_min();
  test_shr_i32_i32_normal_max();
  test_shr_i32_i32_normal_zero();
  test_shr_i32_i32_normal_normal();
  test_shr_i32_i32_normal_min();
  test_shr_i32_i32_min_max();
  test_shr_i32_i32_min_zero();
  test_shr_i32_i32_min_normal();
  test_shr_i32_i32_min_min();

}
// ----------------------------------------------------------------