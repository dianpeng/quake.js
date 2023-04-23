{
  function eqObj(lhs, rhs) {
    return JSON.stringify(lhs) == JSON.stringify(rhs);
  }

  function foo1() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = {};
    }
    yes(eqObj(test_value, {}));
  }
  foo1();

  function foo2() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = {'a': 1};
    }
    yes(eqObj(test_value, {'a': 1}));
  }
  foo2();


  function foo3() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = {'a': {}};
    }
    yes(eqObj(test_value, {'a': {}}));
  }
  foo3();

  function foo4() {
    let test_value = undefined;
    for (let i = 0; i < 50; i++) {
      test_value = {'a': 'a'};
    }
    yes(eqObj(test_value, {'a': 'a'}));
  }
  foo4();

  function foo5() {
    let test_value = undefined;
    let xxx = 'Yeah';
    for (let i = 0; i < 50; i++) {
      test_value = {[xxx]: 'a'};
    }
    yes(eqObj(test_value, {[xxx]: 'a'}));
  }
  foo5();


  function foo6() {
    let test_value = undefined;
    let xxx = {};
    for (let i = 0; i < 50; i++) {
      test_value = {[xxx]: 'a'};
    }
    yes(eqObj(test_value, {[xxx]: 'a'}));
  }
  foo6();

  function foo7() {
    let test_value = undefined;
    let xxx = true;
    for (let i = 0; i < 50; i++) {
      test_value = {[xxx]: 'a'};
    }
    yes(eqObj(test_value, {[xxx]: 'a'}));
  }
  foo7();

  function foo8() {
    let test_value = undefined;
    let xxx = true;
    for (let i = 0; i < 50; i++) {
      test_value = {[xxx]: [[[[[[[[[[[[[[[]]]]]]]]]]]]]]]};
    }
    yes(eqObj(test_value, {[xxx]: [[[[[[[[[[[[[[[]]]]]]]]]]]]]]]}));
  }
  foo8();
}
