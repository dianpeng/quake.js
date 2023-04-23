// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{


function test_get_field_f64_normal() {
  dump("testing: test_get_field_f64_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][13.0];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][13.0]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_normal() {
  dump("testing: test_set_field_f64_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][13.0] = val;
  let expect = obj.b[0].c.g[obji][13.0];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][13.0] = val;
      test_value = obj.b[0].c.g[obji][13.0];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_max() {
  dump("testing: test_get_field_f64_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][1.79769313486e+308];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][1.79769313486e+308]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_max() {
  dump("testing: test_set_field_f64_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][1.79769313486e+308] = val;
  let expect = obj.b[0].c.g[obji][1.79769313486e+308];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][1.79769313486e+308] = val;
      test_value = obj.b[0].c.g[obji][1.79769313486e+308];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_min() {
  dump("testing: test_get_field_f64_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][-1.79769313486e+308];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][-1.79769313486e+308]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_min() {
  dump("testing: test_set_field_f64_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][-1.79769313486e+308] = val;
  let expect = obj.b[0].c.g[obji][-1.79769313486e+308];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][-1.79769313486e+308] = val;
      test_value = obj.b[0].c.g[obji][-1.79769313486e+308];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_nan() {
  dump("testing: test_get_field_f64_nan");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][NaN];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][NaN]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_nan() {
  dump("testing: test_set_field_f64_nan");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][NaN] = val;
  let expect = obj.b[0].c.g[obji][NaN];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][NaN] = val;
      test_value = obj.b[0].c.g[obji][NaN];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_pinf() {
  dump("testing: test_get_field_f64_pinf");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][-Infinity];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][-Infinity]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_pinf() {
  dump("testing: test_set_field_f64_pinf");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][-Infinity] = val;
  let expect = obj.b[0].c.g[obji][-Infinity];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][-Infinity] = val;
      test_value = obj.b[0].c.g[obji][-Infinity];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_zero() {
  dump("testing: test_get_field_f64_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][2.22507385851e-308];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][2.22507385851e-308]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_zero() {
  dump("testing: test_set_field_f64_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][2.22507385851e-308] = val;
  let expect = obj.b[0].c.g[obji][2.22507385851e-308];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][2.22507385851e-308] = val;
      test_value = obj.b[0].c.g[obji][2.22507385851e-308];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_f64_ninf() {
  dump("testing: test_get_field_f64_ninf");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][Infinity];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][Infinity]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_f64_ninf() {
  dump("testing: test_set_field_f64_ninf");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][Infinity] = val;
  let expect = obj.b[0].c.g[obji][Infinity];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][Infinity] = val;
      test_value = obj.b[0].c.g[obji][Infinity];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_i32_max() {
  dump("testing: test_get_field_i32_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][2147483647];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][2147483647]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_i32_max() {
  dump("testing: test_set_field_i32_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][2147483647] = val;
  let expect = obj.b[0].c.g[obji][2147483647];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][2147483647] = val;
      test_value = obj.b[0].c.g[obji][2147483647];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_i32_zero() {
  dump("testing: test_get_field_i32_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][0];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][0]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_i32_zero() {
  dump("testing: test_set_field_i32_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][0] = val;
  let expect = obj.b[0].c.g[obji][0];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][0] = val;
      test_value = obj.b[0].c.g[obji][0];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_i32_normal() {
  dump("testing: test_get_field_i32_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][14];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][14]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_i32_normal() {
  dump("testing: test_set_field_i32_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][14] = val;
  let expect = obj.b[0].c.g[obji][14];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][14] = val;
      test_value = obj.b[0].c.g[obji][14];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_i32_min() {
  dump("testing: test_get_field_i32_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][-2147483648];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][-2147483648]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_i32_min() {
  dump("testing: test_set_field_i32_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][-2147483648] = val;
  let expect = obj.b[0].c.g[obji][-2147483648];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][-2147483648] = val;
      test_value = obj.b[0].c.g[obji][-2147483648];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_bool_max() {
  dump("testing: test_get_field_bool_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][true];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][true]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_bool_max() {
  dump("testing: test_set_field_bool_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][true] = val;
  let expect = obj.b[0].c.g[obji][true];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][true] = val;
      test_value = obj.b[0].c.g[obji][true];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_bool_zero() {
  dump("testing: test_get_field_bool_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][false];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][false]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_bool_zero() {
  dump("testing: test_set_field_bool_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][false] = val;
  let expect = obj.b[0].c.g[obji][false];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][false] = val;
      test_value = obj.b[0].c.g[obji][false];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_bool_normal() {
  dump("testing: test_get_field_bool_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][true];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][true]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_bool_normal() {
  dump("testing: test_set_field_bool_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][true] = val;
  let expect = obj.b[0].c.g[obji][true];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][true] = val;
      test_value = obj.b[0].c.g[obji][true];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_bool_min() {
  dump("testing: test_get_field_bool_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][false];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][false]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_bool_min() {
  dump("testing: test_set_field_bool_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][false] = val;
  let expect = obj.b[0].c.g[obji][false];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][false] = val;
      test_value = obj.b[0].c.g[obji][false];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_undefined_or_null_max() {
  dump("testing: test_get_field_undefined_or_null_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][null];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][null]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_undefined_or_null_max() {
  dump("testing: test_set_field_undefined_or_null_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][null] = val;
  let expect = obj.b[0].c.g[obji][null];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][null] = val;
      test_value = obj.b[0].c.g[obji][null];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_undefined_or_null_zero() {
  dump("testing: test_get_field_undefined_or_null_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][undefined];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][undefined]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_undefined_or_null_zero() {
  dump("testing: test_set_field_undefined_or_null_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][undefined] = val;
  let expect = obj.b[0].c.g[obji][undefined];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][undefined] = val;
      test_value = obj.b[0].c.g[obji][undefined];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_undefined_or_null_normal() {
  dump("testing: test_get_field_undefined_or_null_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][null];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][null]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_undefined_or_null_normal() {
  dump("testing: test_set_field_undefined_or_null_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][null] = val;
  let expect = obj.b[0].c.g[obji][null];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][null] = val;
      test_value = obj.b[0].c.g[obji][null];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_undefined_or_null_min() {
  dump("testing: test_get_field_undefined_or_null_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][undefined];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][undefined]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_undefined_or_null_min() {
  dump("testing: test_set_field_undefined_or_null_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][undefined] = val;
  let expect = obj.b[0].c.g[obji][undefined];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][undefined] = val;
      test_value = obj.b[0].c.g[obji][undefined];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_object_max() {
  dump("testing: test_get_field_object_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][{'a':1}];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][{'a':1}]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_object_max() {
  dump("testing: test_set_field_object_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][{'a':1}] = val;
  let expect = obj.b[0].c.g[obji][{'a':1}];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][{'a':1}] = val;
      test_value = obj.b[0].c.g[obji][{'a':1}];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_object_zero() {
  dump("testing: test_get_field_object_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][{}];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][{}]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_object_zero() {
  dump("testing: test_set_field_object_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][{}] = val;
  let expect = obj.b[0].c.g[obji][{}];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][{}] = val;
      test_value = obj.b[0].c.g[obji][{}];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_object_normal() {
  dump("testing: test_get_field_object_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][{'a':100}];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][{'a':100}]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_object_normal() {
  dump("testing: test_set_field_object_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][{'a':100}] = val;
  let expect = obj.b[0].c.g[obji][{'a':100}];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][{'a':100}] = val;
      test_value = obj.b[0].c.g[obji][{'a':100}];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_object_min() {
  dump("testing: test_get_field_object_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][{}];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji][{}]
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_object_min() {
  dump("testing: test_set_field_object_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][{}] = val;
  let expect = obj.b[0].c.g[obji][{}];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][{}] = val;
      test_value = obj.b[0].c.g[obji][{}];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_stri32_max() {
  dump("testing: test_get_field_stri32_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['2147483647'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['2147483647']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_stri32_max() {
  dump("testing: test_set_field_stri32_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['2147483647'] = val;
  let expect = obj.b[0].c.g[obji]['2147483647'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['2147483647'] = val;
      test_value = obj.b[0].c.g[obji]['2147483647'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_stri32_zero() {
  dump("testing: test_get_field_stri32_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['1'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['1']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_stri32_zero() {
  dump("testing: test_set_field_stri32_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['1'] = val;
  let expect = obj.b[0].c.g[obji]['1'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['1'] = val;
      test_value = obj.b[0].c.g[obji]['1'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_stri32_normal() {
  dump("testing: test_get_field_stri32_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['1000'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['1000']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_stri32_normal() {
  dump("testing: test_set_field_stri32_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['1000'] = val;
  let expect = obj.b[0].c.g[obji]['1000'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['1000'] = val;
      test_value = obj.b[0].c.g[obji]['1000'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_stri32_min() {
  dump("testing: test_get_field_stri32_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['-2147483648'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['-2147483648']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_stri32_min() {
  dump("testing: test_set_field_stri32_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['-2147483648'] = val;
  let expect = obj.b[0].c.g[obji]['-2147483648'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['-2147483648'] = val;
      test_value = obj.b[0].c.g[obji]['-2147483648'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_strf64_max() {
  dump("testing: test_get_field_strf64_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['1.79769313486231570e+308'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['1.79769313486231570e+308']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_strf64_max() {
  dump("testing: test_set_field_strf64_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['1.79769313486231570e+308'] = val;
  let expect = obj.b[0].c.g[obji]['1.79769313486231570e+308'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['1.79769313486231570e+308'] = val;
      test_value = obj.b[0].c.g[obji]['1.79769313486231570e+308'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_strf64_zero() {
  dump("testing: test_get_field_strf64_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['2.2250738585072014E-308'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['2.2250738585072014E-308']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_strf64_zero() {
  dump("testing: test_set_field_strf64_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['2.2250738585072014E-308'] = val;
  let expect = obj.b[0].c.g[obji]['2.2250738585072014E-308'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['2.2250738585072014E-308'] = val;
      test_value = obj.b[0].c.g[obji]['2.2250738585072014E-308'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_strf64_normal() {
  dump("testing: test_get_field_strf64_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['13.2'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['13.2']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_strf64_normal() {
  dump("testing: test_set_field_strf64_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['13.2'] = val;
  let expect = obj.b[0].c.g[obji]['13.2'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['13.2'] = val;
      test_value = obj.b[0].c.g[obji]['13.2'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_strf64_min() {
  dump("testing: test_get_field_strf64_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['-2.2250738585072014E-308'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['-2.2250738585072014E-308']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_strf64_min() {
  dump("testing: test_set_field_strf64_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['-2.2250738585072014E-308'] = val;
  let expect = obj.b[0].c.g[obji]['-2.2250738585072014E-308'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['-2.2250738585072014E-308'] = val;
      test_value = obj.b[0].c.g[obji]['-2.2250738585072014E-308'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_string_max() {
  dump("testing: test_get_field_string_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_string_max() {
  dump("testing: test_set_field_string_max");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'] = val;
  let expect = obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'] = val;
      test_value = obj.b[0].c.g[obji]['aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_string_zero() {
  dump("testing: test_get_field_string_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][''];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_string_zero() {
  dump("testing: test_set_field_string_zero");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][''] = val;
  let expect = obj.b[0].c.g[obji][''];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][''] = val;
      test_value = obj.b[0].c.g[obji][''];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_string_normal() {
  dump("testing: test_get_field_string_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji]['aaaabbccc'];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['aaaabbccc']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_string_normal() {
  dump("testing: test_set_field_string_normal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji]['aaaabbccc'] = val;
  let expect = obj.b[0].c.g[obji]['aaaabbccc'];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji]['aaaabbccc'] = val;
      test_value = obj.b[0].c.g[obji]['aaaabbccc'];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_string_min() {
  dump("testing: test_get_field_string_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji][''];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji]['']
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_string_min() {
  dump("testing: test_set_field_string_min");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji][''] = val;
  let expect = obj.b[0].c.g[obji][''];

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji][''] = val;
      test_value = obj.b[0].c.g[obji][''];
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_get_field_a_literal() {
  dump("testing: test_get_field_a_literal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let expect = obj.b[0].c.g[obji].a;

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {
    test_value = obj.b[0].c.g[obji].a
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}



function test_set_field_a_literal() {
  dump("testing: test_set_field_a_literal");

  let obji = {'xxx':111};
  let obj = 

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
;
  let val = {};

  let test_value = undefined;
  obj.b[0].c.g[obji].a = val;
  let expect = obj.b[0].c.g[obji].a;

  for (let i = 0; i < 50; i++) {
      obj.b[0].c.g[obji].a = val;
      test_value = obj.b[0].c.g[obji].a;
  }

  if (isNaN(test_value)) {
    yes(isNaN(expect));
  } else {
    yes(test_value === expect);
  }
}

  test_get_field_f64_normal();
  test_set_field_f64_normal();
  test_get_field_f64_max();
  test_set_field_f64_max();
  test_get_field_f64_min();
  test_set_field_f64_min();
  test_get_field_f64_nan();
  test_set_field_f64_nan();
  test_get_field_f64_pinf();
  test_set_field_f64_pinf();
  test_get_field_f64_zero();
  test_set_field_f64_zero();
  test_get_field_f64_ninf();
  test_set_field_f64_ninf();
  test_get_field_i32_max();
  test_set_field_i32_max();
  test_get_field_i32_zero();
  test_set_field_i32_zero();
  test_get_field_i32_normal();
  test_set_field_i32_normal();
  test_get_field_i32_min();
  test_set_field_i32_min();
  test_get_field_bool_max();
  test_set_field_bool_max();
  test_get_field_bool_zero();
  test_set_field_bool_zero();
  test_get_field_bool_normal();
  test_set_field_bool_normal();
  test_get_field_bool_min();
  test_set_field_bool_min();
  test_get_field_undefined_or_null_max();
  test_set_field_undefined_or_null_max();
  test_get_field_undefined_or_null_zero();
  test_set_field_undefined_or_null_zero();
  test_get_field_undefined_or_null_normal();
  test_set_field_undefined_or_null_normal();
  test_get_field_undefined_or_null_min();
  test_set_field_undefined_or_null_min();
  test_get_field_object_max();
  test_set_field_object_max();
  test_get_field_object_zero();
  test_set_field_object_zero();
  test_get_field_object_normal();
  test_set_field_object_normal();
  test_get_field_object_min();
  test_set_field_object_min();
  test_get_field_stri32_max();
  test_set_field_stri32_max();
  test_get_field_stri32_zero();
  test_set_field_stri32_zero();
  test_get_field_stri32_normal();
  test_set_field_stri32_normal();
  test_get_field_stri32_min();
  test_set_field_stri32_min();
  test_get_field_strf64_max();
  test_set_field_strf64_max();
  test_get_field_strf64_zero();
  test_set_field_strf64_zero();
  test_get_field_strf64_normal();
  test_set_field_strf64_normal();
  test_get_field_strf64_min();
  test_set_field_strf64_min();
  test_get_field_string_max();
  test_set_field_string_max();
  test_get_field_string_zero();
  test_set_field_string_zero();
  test_get_field_string_normal();
  test_set_field_string_normal();
  test_get_field_string_min();
  test_set_field_string_min();
  test_get_field_a_literal();
  test_set_field_a_literal();

}
// ----------------------------------------------------------------