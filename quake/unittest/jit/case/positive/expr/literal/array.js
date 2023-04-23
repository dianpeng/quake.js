{
  function eqArray(lhs, rhs) {
    let ljson = JSON.stringify(lhs);
    let rjson = JSON.stringify(rhs);
    return ljson == rjson;
  }
  function foo1() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [];
    }
    yes(eqArray(test_value, []));
  }
  foo1();

  function foo2() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [1];
    }
    yes(eqArray(test_value, [1]));
  }
  foo2();

  function foo3() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [1, true, false, undefined, null];
    }
    yes(eqArray(test_value, [1, true, false, undefined, null]));
  }
  foo3();

  function foo10() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [''];
    }
    yes(eqArray(test_value, ['']));
  }
  foo10();

  function foo4() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [1, true, false, undefined, null, ''];
    }
    yes(eqArray(test_value, [1, true, false, undefined, null, '']));
  }
  foo4();

  function foo5() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = [1, [[[[[[[[[[[[[]]]]]]]]]]]]], {'a': [[[{'b': [[[]]]}]]]}];
    }

    yes(eqArray(
        test_value,
        [1, [[[[[[[[[[[[[]]]]]]]]]]]]], {'a': [[[{'b': [[[]]]}]]]}]));
  }
  foo5();
}
