// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_get_index_f64_normal_0() {
  dump("testing: test_get_index_f64_normal_0");

  let obj = [];
  let idx = 13.0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_normal_0() {
  dump("testing: test_set_index_f64_normal_0");

  let obj = [];
  let idx = 13.0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_normal_1() {
  dump("testing: test_get_index_f64_normal_1");

  let obj = [1];
  let idx = 13.0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_normal_1() {
  dump("testing: test_set_index_f64_normal_1");

  let obj = [1];
  let idx = 13.0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_normal_2() {
  dump("testing: test_get_index_f64_normal_2");

  let obj = [1, 2];
  let idx = 13.0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_normal_2() {
  dump("testing: test_set_index_f64_normal_2");

  let obj = [1, 2];
  let idx = 13.0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_max_0() {
  dump("testing: test_get_index_f64_max_0");

  let obj = [];
  let idx = 1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_max_0() {
  dump("testing: test_set_index_f64_max_0");

  let obj = [];
  let idx = 1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_max_1() {
  dump("testing: test_get_index_f64_max_1");

  let obj = [1];
  let idx = 1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_max_1() {
  dump("testing: test_set_index_f64_max_1");

  let obj = [1];
  let idx = 1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_max_2() {
  dump("testing: test_get_index_f64_max_2");

  let obj = [1, 2];
  let idx = 1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_max_2() {
  dump("testing: test_set_index_f64_max_2");

  let obj = [1, 2];
  let idx = 1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_min_0() {
  dump("testing: test_get_index_f64_min_0");

  let obj = [];
  let idx = -1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_min_0() {
  dump("testing: test_set_index_f64_min_0");

  let obj = [];
  let idx = -1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_min_1() {
  dump("testing: test_get_index_f64_min_1");

  let obj = [1];
  let idx = -1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_min_1() {
  dump("testing: test_set_index_f64_min_1");

  let obj = [1];
  let idx = -1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_min_2() {
  dump("testing: test_get_index_f64_min_2");

  let obj = [1, 2];
  let idx = -1.79769313486e+308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_min_2() {
  dump("testing: test_set_index_f64_min_2");

  let obj = [1, 2];
  let idx = -1.79769313486e+308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_nan_0() {
  dump("testing: test_get_index_f64_nan_0");

  let obj = [];
  let idx = NaN;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_nan_0() {
  dump("testing: test_set_index_f64_nan_0");

  let obj = [];
  let idx = NaN;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_nan_1() {
  dump("testing: test_get_index_f64_nan_1");

  let obj = [1];
  let idx = NaN;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_nan_1() {
  dump("testing: test_set_index_f64_nan_1");

  let obj = [1];
  let idx = NaN;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_nan_2() {
  dump("testing: test_get_index_f64_nan_2");

  let obj = [1, 2];
  let idx = NaN;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_nan_2() {
  dump("testing: test_set_index_f64_nan_2");

  let obj = [1, 2];
  let idx = NaN;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_pinf_0() {
  dump("testing: test_get_index_f64_pinf_0");

  let obj = [];
  let idx = -Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_pinf_0() {
  dump("testing: test_set_index_f64_pinf_0");

  let obj = [];
  let idx = -Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_pinf_1() {
  dump("testing: test_get_index_f64_pinf_1");

  let obj = [1];
  let idx = -Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_pinf_1() {
  dump("testing: test_set_index_f64_pinf_1");

  let obj = [1];
  let idx = -Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_pinf_2() {
  dump("testing: test_get_index_f64_pinf_2");

  let obj = [1, 2];
  let idx = -Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_pinf_2() {
  dump("testing: test_set_index_f64_pinf_2");

  let obj = [1, 2];
  let idx = -Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_zero_0() {
  dump("testing: test_get_index_f64_zero_0");

  let obj = [];
  let idx = 2.22507385851e-308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_zero_0() {
  dump("testing: test_set_index_f64_zero_0");

  let obj = [];
  let idx = 2.22507385851e-308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_zero_1() {
  dump("testing: test_get_index_f64_zero_1");

  let obj = [1];
  let idx = 2.22507385851e-308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_zero_1() {
  dump("testing: test_set_index_f64_zero_1");

  let obj = [1];
  let idx = 2.22507385851e-308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_zero_2() {
  dump("testing: test_get_index_f64_zero_2");

  let obj = [1, 2];
  let idx = 2.22507385851e-308;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_zero_2() {
  dump("testing: test_set_index_f64_zero_2");

  let obj = [1, 2];
  let idx = 2.22507385851e-308;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_ninf_0() {
  dump("testing: test_get_index_f64_ninf_0");

  let obj = [];
  let idx = Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_ninf_0() {
  dump("testing: test_set_index_f64_ninf_0");

  let obj = [];
  let idx = Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_ninf_1() {
  dump("testing: test_get_index_f64_ninf_1");

  let obj = [1];
  let idx = Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_ninf_1() {
  dump("testing: test_set_index_f64_ninf_1");

  let obj = [1];
  let idx = Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_f64_ninf_2() {
  dump("testing: test_get_index_f64_ninf_2");

  let obj = [1, 2];
  let idx = Infinity;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_f64_ninf_2() {
  dump("testing: test_set_index_f64_ninf_2");

  let obj = [1, 2];
  let idx = Infinity;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_max_0() {
  dump("testing: test_get_index_i32_max_0");

  let obj = [];
  let idx = 2147483647;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_max_0() {
  dump("testing: test_set_index_i32_max_0");

  let obj = [];
  let idx = 2147483647;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_max_1() {
  dump("testing: test_get_index_i32_max_1");

  let obj = [1];
  let idx = 2147483647;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_max_1() {
  dump("testing: test_set_index_i32_max_1");

  let obj = [1];
  let idx = 2147483647;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_max_2() {
  dump("testing: test_get_index_i32_max_2");

  let obj = [1, 2];
  let idx = 2147483647;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_max_2() {
  dump("testing: test_set_index_i32_max_2");

  let obj = [1, 2];
  let idx = 2147483647;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_zero_0() {
  dump("testing: test_get_index_i32_zero_0");

  let obj = [];
  let idx = 0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_zero_0() {
  dump("testing: test_set_index_i32_zero_0");

  let obj = [];
  let idx = 0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_zero_1() {
  dump("testing: test_get_index_i32_zero_1");

  let obj = [1];
  let idx = 0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_zero_1() {
  dump("testing: test_set_index_i32_zero_1");

  let obj = [1];
  let idx = 0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_zero_2() {
  dump("testing: test_get_index_i32_zero_2");

  let obj = [1, 2];
  let idx = 0;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_zero_2() {
  dump("testing: test_set_index_i32_zero_2");

  let obj = [1, 2];
  let idx = 0;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_normal_0() {
  dump("testing: test_get_index_i32_normal_0");

  let obj = [];
  let idx = 14;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_normal_0() {
  dump("testing: test_set_index_i32_normal_0");

  let obj = [];
  let idx = 14;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_normal_1() {
  dump("testing: test_get_index_i32_normal_1");

  let obj = [1];
  let idx = 14;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_normal_1() {
  dump("testing: test_set_index_i32_normal_1");

  let obj = [1];
  let idx = 14;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_normal_2() {
  dump("testing: test_get_index_i32_normal_2");

  let obj = [1, 2];
  let idx = 14;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_normal_2() {
  dump("testing: test_set_index_i32_normal_2");

  let obj = [1, 2];
  let idx = 14;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_min_0() {
  dump("testing: test_get_index_i32_min_0");

  let obj = [];
  let idx = -2147483648;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_min_0() {
  dump("testing: test_set_index_i32_min_0");

  let obj = [];
  let idx = -2147483648;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_min_1() {
  dump("testing: test_get_index_i32_min_1");

  let obj = [1];
  let idx = -2147483648;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_min_1() {
  dump("testing: test_set_index_i32_min_1");

  let obj = [1];
  let idx = -2147483648;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_i32_min_2() {
  dump("testing: test_get_index_i32_min_2");

  let obj = [1, 2];
  let idx = -2147483648;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_i32_min_2() {
  dump("testing: test_set_index_i32_min_2");

  let obj = [1, 2];
  let idx = -2147483648;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_max_0() {
  dump("testing: test_get_index_bool_max_0");

  let obj = [];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_max_0() {
  dump("testing: test_set_index_bool_max_0");

  let obj = [];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_max_1() {
  dump("testing: test_get_index_bool_max_1");

  let obj = [1];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_max_1() {
  dump("testing: test_set_index_bool_max_1");

  let obj = [1];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_max_2() {
  dump("testing: test_get_index_bool_max_2");

  let obj = [1, 2];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_max_2() {
  dump("testing: test_set_index_bool_max_2");

  let obj = [1, 2];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_zero_0() {
  dump("testing: test_get_index_bool_zero_0");

  let obj = [];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_zero_0() {
  dump("testing: test_set_index_bool_zero_0");

  let obj = [];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_zero_1() {
  dump("testing: test_get_index_bool_zero_1");

  let obj = [1];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_zero_1() {
  dump("testing: test_set_index_bool_zero_1");

  let obj = [1];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_zero_2() {
  dump("testing: test_get_index_bool_zero_2");

  let obj = [1, 2];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_zero_2() {
  dump("testing: test_set_index_bool_zero_2");

  let obj = [1, 2];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_normal_0() {
  dump("testing: test_get_index_bool_normal_0");

  let obj = [];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_normal_0() {
  dump("testing: test_set_index_bool_normal_0");

  let obj = [];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_normal_1() {
  dump("testing: test_get_index_bool_normal_1");

  let obj = [1];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_normal_1() {
  dump("testing: test_set_index_bool_normal_1");

  let obj = [1];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_normal_2() {
  dump("testing: test_get_index_bool_normal_2");

  let obj = [1, 2];
  let idx = true;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_normal_2() {
  dump("testing: test_set_index_bool_normal_2");

  let obj = [1, 2];
  let idx = true;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_min_0() {
  dump("testing: test_get_index_bool_min_0");

  let obj = [];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_min_0() {
  dump("testing: test_set_index_bool_min_0");

  let obj = [];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_min_1() {
  dump("testing: test_get_index_bool_min_1");

  let obj = [1];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_min_1() {
  dump("testing: test_set_index_bool_min_1");

  let obj = [1];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_bool_min_2() {
  dump("testing: test_get_index_bool_min_2");

  let obj = [1, 2];
  let idx = false;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_bool_min_2() {
  dump("testing: test_set_index_bool_min_2");

  let obj = [1, 2];
  let idx = false;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_max_0() {
  dump("testing: test_get_index_undefined_or_null_max_0");

  let obj = [];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_max_0() {
  dump("testing: test_set_index_undefined_or_null_max_0");

  let obj = [];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_max_1() {
  dump("testing: test_get_index_undefined_or_null_max_1");

  let obj = [1];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_max_1() {
  dump("testing: test_set_index_undefined_or_null_max_1");

  let obj = [1];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_max_2() {
  dump("testing: test_get_index_undefined_or_null_max_2");

  let obj = [1, 2];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_max_2() {
  dump("testing: test_set_index_undefined_or_null_max_2");

  let obj = [1, 2];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_zero_0() {
  dump("testing: test_get_index_undefined_or_null_zero_0");

  let obj = [];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_zero_0() {
  dump("testing: test_set_index_undefined_or_null_zero_0");

  let obj = [];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_zero_1() {
  dump("testing: test_get_index_undefined_or_null_zero_1");

  let obj = [1];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_zero_1() {
  dump("testing: test_set_index_undefined_or_null_zero_1");

  let obj = [1];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_zero_2() {
  dump("testing: test_get_index_undefined_or_null_zero_2");

  let obj = [1, 2];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_zero_2() {
  dump("testing: test_set_index_undefined_or_null_zero_2");

  let obj = [1, 2];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_normal_0() {
  dump("testing: test_get_index_undefined_or_null_normal_0");

  let obj = [];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_normal_0() {
  dump("testing: test_set_index_undefined_or_null_normal_0");

  let obj = [];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_normal_1() {
  dump("testing: test_get_index_undefined_or_null_normal_1");

  let obj = [1];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_normal_1() {
  dump("testing: test_set_index_undefined_or_null_normal_1");

  let obj = [1];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_normal_2() {
  dump("testing: test_get_index_undefined_or_null_normal_2");

  let obj = [1, 2];
  let idx = null;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_normal_2() {
  dump("testing: test_set_index_undefined_or_null_normal_2");

  let obj = [1, 2];
  let idx = null;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_min_0() {
  dump("testing: test_get_index_undefined_or_null_min_0");

  let obj = [];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_min_0() {
  dump("testing: test_set_index_undefined_or_null_min_0");

  let obj = [];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_min_1() {
  dump("testing: test_get_index_undefined_or_null_min_1");

  let obj = [1];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_min_1() {
  dump("testing: test_set_index_undefined_or_null_min_1");

  let obj = [1];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_undefined_or_null_min_2() {
  dump("testing: test_get_index_undefined_or_null_min_2");

  let obj = [1, 2];
  let idx = undefined;
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_undefined_or_null_min_2() {
  dump("testing: test_set_index_undefined_or_null_min_2");

  let obj = [1, 2];
  let idx = undefined;
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_max_0() {
  dump("testing: test_get_index_object_max_0");

  let obj = [];
  let idx = {'a':1};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_max_0() {
  dump("testing: test_set_index_object_max_0");

  let obj = [];
  let idx = {'a':1};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_max_1() {
  dump("testing: test_get_index_object_max_1");

  let obj = [1];
  let idx = {'a':1};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_max_1() {
  dump("testing: test_set_index_object_max_1");

  let obj = [1];
  let idx = {'a':1};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_max_2() {
  dump("testing: test_get_index_object_max_2");

  let obj = [1, 2];
  let idx = {'a':1};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_max_2() {
  dump("testing: test_set_index_object_max_2");

  let obj = [1, 2];
  let idx = {'a':1};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_zero_0() {
  dump("testing: test_get_index_object_zero_0");

  let obj = [];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_zero_0() {
  dump("testing: test_set_index_object_zero_0");

  let obj = [];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_zero_1() {
  dump("testing: test_get_index_object_zero_1");

  let obj = [1];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_zero_1() {
  dump("testing: test_set_index_object_zero_1");

  let obj = [1];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_zero_2() {
  dump("testing: test_get_index_object_zero_2");

  let obj = [1, 2];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_zero_2() {
  dump("testing: test_set_index_object_zero_2");

  let obj = [1, 2];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_normal_0() {
  dump("testing: test_get_index_object_normal_0");

  let obj = [];
  let idx = {'a':100};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_normal_0() {
  dump("testing: test_set_index_object_normal_0");

  let obj = [];
  let idx = {'a':100};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_normal_1() {
  dump("testing: test_get_index_object_normal_1");

  let obj = [1];
  let idx = {'a':100};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_normal_1() {
  dump("testing: test_set_index_object_normal_1");

  let obj = [1];
  let idx = {'a':100};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_normal_2() {
  dump("testing: test_get_index_object_normal_2");

  let obj = [1, 2];
  let idx = {'a':100};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_normal_2() {
  dump("testing: test_set_index_object_normal_2");

  let obj = [1, 2];
  let idx = {'a':100};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_min_0() {
  dump("testing: test_get_index_object_min_0");

  let obj = [];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_min_0() {
  dump("testing: test_set_index_object_min_0");

  let obj = [];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_min_1() {
  dump("testing: test_get_index_object_min_1");

  let obj = [1];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_min_1() {
  dump("testing: test_set_index_object_min_1");

  let obj = [1];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_object_min_2() {
  dump("testing: test_get_index_object_min_2");

  let obj = [1, 2];
  let idx = {};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_object_min_2() {
  dump("testing: test_set_index_object_min_2");

  let obj = [1, 2];
  let idx = {};
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_max_0() {
  dump("testing: test_get_index_stri32_max_0");

  let obj = [];
  let idx = '2147483647';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_max_0() {
  dump("testing: test_set_index_stri32_max_0");

  let obj = [];
  let idx = '2147483647';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_max_1() {
  dump("testing: test_get_index_stri32_max_1");

  let obj = [1];
  let idx = '2147483647';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_max_1() {
  dump("testing: test_set_index_stri32_max_1");

  let obj = [1];
  let idx = '2147483647';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_max_2() {
  dump("testing: test_get_index_stri32_max_2");

  let obj = [1, 2];
  let idx = '2147483647';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_max_2() {
  dump("testing: test_set_index_stri32_max_2");

  let obj = [1, 2];
  let idx = '2147483647';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_zero_0() {
  dump("testing: test_get_index_stri32_zero_0");

  let obj = [];
  let idx = '1';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_zero_0() {
  dump("testing: test_set_index_stri32_zero_0");

  let obj = [];
  let idx = '1';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_zero_1() {
  dump("testing: test_get_index_stri32_zero_1");

  let obj = [1];
  let idx = '1';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_zero_1() {
  dump("testing: test_set_index_stri32_zero_1");

  let obj = [1];
  let idx = '1';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_zero_2() {
  dump("testing: test_get_index_stri32_zero_2");

  let obj = [1, 2];
  let idx = '1';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_zero_2() {
  dump("testing: test_set_index_stri32_zero_2");

  let obj = [1, 2];
  let idx = '1';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_normal_0() {
  dump("testing: test_get_index_stri32_normal_0");

  let obj = [];
  let idx = '1000';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_normal_0() {
  dump("testing: test_set_index_stri32_normal_0");

  let obj = [];
  let idx = '1000';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_normal_1() {
  dump("testing: test_get_index_stri32_normal_1");

  let obj = [1];
  let idx = '1000';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_normal_1() {
  dump("testing: test_set_index_stri32_normal_1");

  let obj = [1];
  let idx = '1000';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_normal_2() {
  dump("testing: test_get_index_stri32_normal_2");

  let obj = [1, 2];
  let idx = '1000';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_normal_2() {
  dump("testing: test_set_index_stri32_normal_2");

  let obj = [1, 2];
  let idx = '1000';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_min_0() {
  dump("testing: test_get_index_stri32_min_0");

  let obj = [];
  let idx = '-2147483648';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_min_0() {
  dump("testing: test_set_index_stri32_min_0");

  let obj = [];
  let idx = '-2147483648';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_min_1() {
  dump("testing: test_get_index_stri32_min_1");

  let obj = [1];
  let idx = '-2147483648';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_min_1() {
  dump("testing: test_set_index_stri32_min_1");

  let obj = [1];
  let idx = '-2147483648';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_stri32_min_2() {
  dump("testing: test_get_index_stri32_min_2");

  let obj = [1, 2];
  let idx = '-2147483648';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_stri32_min_2() {
  dump("testing: test_set_index_stri32_min_2");

  let obj = [1, 2];
  let idx = '-2147483648';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_max_0() {
  dump("testing: test_get_index_strf64_max_0");

  let obj = [];
  let idx = '1.79769313486231570e+308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_max_0() {
  dump("testing: test_set_index_strf64_max_0");

  let obj = [];
  let idx = '1.79769313486231570e+308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_max_1() {
  dump("testing: test_get_index_strf64_max_1");

  let obj = [1];
  let idx = '1.79769313486231570e+308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_max_1() {
  dump("testing: test_set_index_strf64_max_1");

  let obj = [1];
  let idx = '1.79769313486231570e+308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_max_2() {
  dump("testing: test_get_index_strf64_max_2");

  let obj = [1, 2];
  let idx = '1.79769313486231570e+308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_max_2() {
  dump("testing: test_set_index_strf64_max_2");

  let obj = [1, 2];
  let idx = '1.79769313486231570e+308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_zero_0() {
  dump("testing: test_get_index_strf64_zero_0");

  let obj = [];
  let idx = '2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_zero_0() {
  dump("testing: test_set_index_strf64_zero_0");

  let obj = [];
  let idx = '2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_zero_1() {
  dump("testing: test_get_index_strf64_zero_1");

  let obj = [1];
  let idx = '2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_zero_1() {
  dump("testing: test_set_index_strf64_zero_1");

  let obj = [1];
  let idx = '2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_zero_2() {
  dump("testing: test_get_index_strf64_zero_2");

  let obj = [1, 2];
  let idx = '2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_zero_2() {
  dump("testing: test_set_index_strf64_zero_2");

  let obj = [1, 2];
  let idx = '2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_normal_0() {
  dump("testing: test_get_index_strf64_normal_0");

  let obj = [];
  let idx = '13.2';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_normal_0() {
  dump("testing: test_set_index_strf64_normal_0");

  let obj = [];
  let idx = '13.2';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_normal_1() {
  dump("testing: test_get_index_strf64_normal_1");

  let obj = [1];
  let idx = '13.2';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_normal_1() {
  dump("testing: test_set_index_strf64_normal_1");

  let obj = [1];
  let idx = '13.2';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_normal_2() {
  dump("testing: test_get_index_strf64_normal_2");

  let obj = [1, 2];
  let idx = '13.2';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_normal_2() {
  dump("testing: test_set_index_strf64_normal_2");

  let obj = [1, 2];
  let idx = '13.2';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_min_0() {
  dump("testing: test_get_index_strf64_min_0");

  let obj = [];
  let idx = '-2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_min_0() {
  dump("testing: test_set_index_strf64_min_0");

  let obj = [];
  let idx = '-2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_min_1() {
  dump("testing: test_get_index_strf64_min_1");

  let obj = [1];
  let idx = '-2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_min_1() {
  dump("testing: test_set_index_strf64_min_1");

  let obj = [1];
  let idx = '-2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_strf64_min_2() {
  dump("testing: test_get_index_strf64_min_2");

  let obj = [1, 2];
  let idx = '-2.2250738585072014E-308';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_strf64_min_2() {
  dump("testing: test_set_index_strf64_min_2");

  let obj = [1, 2];
  let idx = '-2.2250738585072014E-308';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_max_0() {
  dump("testing: test_get_index_string_max_0");

  let obj = [];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_max_0() {
  dump("testing: test_set_index_string_max_0");

  let obj = [];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_max_1() {
  dump("testing: test_get_index_string_max_1");

  let obj = [1];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_max_1() {
  dump("testing: test_set_index_string_max_1");

  let obj = [1];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_max_2() {
  dump("testing: test_get_index_string_max_2");

  let obj = [1, 2];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_max_2() {
  dump("testing: test_set_index_string_max_2");

  let obj = [1, 2];
  let idx = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_zero_0() {
  dump("testing: test_get_index_string_zero_0");

  let obj = [];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_zero_0() {
  dump("testing: test_set_index_string_zero_0");

  let obj = [];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_zero_1() {
  dump("testing: test_get_index_string_zero_1");

  let obj = [1];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_zero_1() {
  dump("testing: test_set_index_string_zero_1");

  let obj = [1];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_zero_2() {
  dump("testing: test_get_index_string_zero_2");

  let obj = [1, 2];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_zero_2() {
  dump("testing: test_set_index_string_zero_2");

  let obj = [1, 2];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_normal_0() {
  dump("testing: test_get_index_string_normal_0");

  let obj = [];
  let idx = 'aaaabbccc';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_normal_0() {
  dump("testing: test_set_index_string_normal_0");

  let obj = [];
  let idx = 'aaaabbccc';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_normal_1() {
  dump("testing: test_get_index_string_normal_1");

  let obj = [1];
  let idx = 'aaaabbccc';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_normal_1() {
  dump("testing: test_set_index_string_normal_1");

  let obj = [1];
  let idx = 'aaaabbccc';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_normal_2() {
  dump("testing: test_get_index_string_normal_2");

  let obj = [1, 2];
  let idx = 'aaaabbccc';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_normal_2() {
  dump("testing: test_set_index_string_normal_2");

  let obj = [1, 2];
  let idx = 'aaaabbccc';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_min_0() {
  dump("testing: test_get_index_string_min_0");

  let obj = [];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_min_0() {
  dump("testing: test_set_index_string_min_0");

  let obj = [];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_min_1() {
  dump("testing: test_get_index_string_min_1");

  let obj = [1];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_min_1() {
  dump("testing: test_set_index_string_min_1");

  let obj = [1];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_index_string_min_2() {
  dump("testing: test_get_index_string_min_2");

  let obj = [1, 2];
  let idx = '';
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_index_string_min_2() {
  dump("testing: test_set_index_string_min_2");

  let obj = [1, 2];
  let idx = '';
  let val = {};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {
    obj[idx] = val;
    test_value = obj[idx];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}

  test_get_index_f64_normal_0();
  test_set_index_f64_normal_0();
  test_get_index_f64_normal_1();
  test_set_index_f64_normal_1();
  test_get_index_f64_normal_2();
  test_set_index_f64_normal_2();
  test_get_index_f64_max_0();
  test_set_index_f64_max_0();
  test_get_index_f64_max_1();
  test_set_index_f64_max_1();
  test_get_index_f64_max_2();
  test_set_index_f64_max_2();
  test_get_index_f64_min_0();
  test_set_index_f64_min_0();
  test_get_index_f64_min_1();
  test_set_index_f64_min_1();
  test_get_index_f64_min_2();
  test_set_index_f64_min_2();
  test_get_index_f64_nan_0();
  test_set_index_f64_nan_0();
  test_get_index_f64_nan_1();
  test_set_index_f64_nan_1();
  test_get_index_f64_nan_2();
  test_set_index_f64_nan_2();
  test_get_index_f64_pinf_0();
  test_set_index_f64_pinf_0();
  test_get_index_f64_pinf_1();
  test_set_index_f64_pinf_1();
  test_get_index_f64_pinf_2();
  test_set_index_f64_pinf_2();
  test_get_index_f64_zero_0();
  test_set_index_f64_zero_0();
  test_get_index_f64_zero_1();
  test_set_index_f64_zero_1();
  test_get_index_f64_zero_2();
  test_set_index_f64_zero_2();
  test_get_index_f64_ninf_0();
  test_set_index_f64_ninf_0();
  test_get_index_f64_ninf_1();
  test_set_index_f64_ninf_1();
  test_get_index_f64_ninf_2();
  test_set_index_f64_ninf_2();
  test_get_index_i32_max_0();
  test_set_index_i32_max_0();
  test_get_index_i32_max_1();
  test_set_index_i32_max_1();
  test_get_index_i32_max_2();
  test_set_index_i32_max_2();
  test_get_index_i32_zero_0();
  test_set_index_i32_zero_0();
  test_get_index_i32_zero_1();
  test_set_index_i32_zero_1();
  test_get_index_i32_zero_2();
  test_set_index_i32_zero_2();
  test_get_index_i32_normal_0();
  test_set_index_i32_normal_0();
  test_get_index_i32_normal_1();
  test_set_index_i32_normal_1();
  test_get_index_i32_normal_2();
  test_set_index_i32_normal_2();
  test_get_index_i32_min_0();
  test_set_index_i32_min_0();
  test_get_index_i32_min_1();
  test_set_index_i32_min_1();
  test_get_index_i32_min_2();
  test_set_index_i32_min_2();
  test_get_index_bool_max_0();
  test_set_index_bool_max_0();
  test_get_index_bool_max_1();
  test_set_index_bool_max_1();
  test_get_index_bool_max_2();
  test_set_index_bool_max_2();
  test_get_index_bool_zero_0();
  test_set_index_bool_zero_0();
  test_get_index_bool_zero_1();
  test_set_index_bool_zero_1();
  test_get_index_bool_zero_2();
  test_set_index_bool_zero_2();
  test_get_index_bool_normal_0();
  test_set_index_bool_normal_0();
  test_get_index_bool_normal_1();
  test_set_index_bool_normal_1();
  test_get_index_bool_normal_2();
  test_set_index_bool_normal_2();
  test_get_index_bool_min_0();
  test_set_index_bool_min_0();
  test_get_index_bool_min_1();
  test_set_index_bool_min_1();
  test_get_index_bool_min_2();
  test_set_index_bool_min_2();
  test_get_index_undefined_or_null_max_0();
  test_set_index_undefined_or_null_max_0();
  test_get_index_undefined_or_null_max_1();
  test_set_index_undefined_or_null_max_1();
  test_get_index_undefined_or_null_max_2();
  test_set_index_undefined_or_null_max_2();
  test_get_index_undefined_or_null_zero_0();
  test_set_index_undefined_or_null_zero_0();
  test_get_index_undefined_or_null_zero_1();
  test_set_index_undefined_or_null_zero_1();
  test_get_index_undefined_or_null_zero_2();
  test_set_index_undefined_or_null_zero_2();
  test_get_index_undefined_or_null_normal_0();
  test_set_index_undefined_or_null_normal_0();
  test_get_index_undefined_or_null_normal_1();
  test_set_index_undefined_or_null_normal_1();
  test_get_index_undefined_or_null_normal_2();
  test_set_index_undefined_or_null_normal_2();
  test_get_index_undefined_or_null_min_0();
  test_set_index_undefined_or_null_min_0();
  test_get_index_undefined_or_null_min_1();
  test_set_index_undefined_or_null_min_1();
  test_get_index_undefined_or_null_min_2();
  test_set_index_undefined_or_null_min_2();
  test_get_index_object_max_0();
  test_set_index_object_max_0();
  test_get_index_object_max_1();
  test_set_index_object_max_1();
  test_get_index_object_max_2();
  test_set_index_object_max_2();
  test_get_index_object_zero_0();
  test_set_index_object_zero_0();
  test_get_index_object_zero_1();
  test_set_index_object_zero_1();
  test_get_index_object_zero_2();
  test_set_index_object_zero_2();
  test_get_index_object_normal_0();
  test_set_index_object_normal_0();
  test_get_index_object_normal_1();
  test_set_index_object_normal_1();
  test_get_index_object_normal_2();
  test_set_index_object_normal_2();
  test_get_index_object_min_0();
  test_set_index_object_min_0();
  test_get_index_object_min_1();
  test_set_index_object_min_1();
  test_get_index_object_min_2();
  test_set_index_object_min_2();
  test_get_index_stri32_max_0();
  test_set_index_stri32_max_0();
  test_get_index_stri32_max_1();
  test_set_index_stri32_max_1();
  test_get_index_stri32_max_2();
  test_set_index_stri32_max_2();
  test_get_index_stri32_zero_0();
  test_set_index_stri32_zero_0();
  test_get_index_stri32_zero_1();
  test_set_index_stri32_zero_1();
  test_get_index_stri32_zero_2();
  test_set_index_stri32_zero_2();
  test_get_index_stri32_normal_0();
  test_set_index_stri32_normal_0();
  test_get_index_stri32_normal_1();
  test_set_index_stri32_normal_1();
  test_get_index_stri32_normal_2();
  test_set_index_stri32_normal_2();
  test_get_index_stri32_min_0();
  test_set_index_stri32_min_0();
  test_get_index_stri32_min_1();
  test_set_index_stri32_min_1();
  test_get_index_stri32_min_2();
  test_set_index_stri32_min_2();
  test_get_index_strf64_max_0();
  test_set_index_strf64_max_0();
  test_get_index_strf64_max_1();
  test_set_index_strf64_max_1();
  test_get_index_strf64_max_2();
  test_set_index_strf64_max_2();
  test_get_index_strf64_zero_0();
  test_set_index_strf64_zero_0();
  test_get_index_strf64_zero_1();
  test_set_index_strf64_zero_1();
  test_get_index_strf64_zero_2();
  test_set_index_strf64_zero_2();
  test_get_index_strf64_normal_0();
  test_set_index_strf64_normal_0();
  test_get_index_strf64_normal_1();
  test_set_index_strf64_normal_1();
  test_get_index_strf64_normal_2();
  test_set_index_strf64_normal_2();
  test_get_index_strf64_min_0();
  test_set_index_strf64_min_0();
  test_get_index_strf64_min_1();
  test_set_index_strf64_min_1();
  test_get_index_strf64_min_2();
  test_set_index_strf64_min_2();
  test_get_index_string_max_0();
  test_set_index_string_max_0();
  test_get_index_string_max_1();
  test_set_index_string_max_1();
  test_get_index_string_max_2();
  test_set_index_string_max_2();
  test_get_index_string_zero_0();
  test_set_index_string_zero_0();
  test_get_index_string_zero_1();
  test_set_index_string_zero_1();
  test_get_index_string_zero_2();
  test_set_index_string_zero_2();
  test_get_index_string_normal_0();
  test_set_index_string_normal_0();
  test_get_index_string_normal_1();
  test_set_index_string_normal_1();
  test_get_index_string_normal_2();
  test_set_index_string_normal_2();
  test_get_index_string_min_0();
  test_set_index_string_min_0();
  test_get_index_string_min_1();
  test_set_index_string_min_1();
  test_get_index_string_min_2();
  test_set_index_string_min_2();

}
// ----------------------------------------------------------------