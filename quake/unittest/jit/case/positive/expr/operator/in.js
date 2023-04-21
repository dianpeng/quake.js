{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(1, []);
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(1, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1('', {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(true, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(undefined, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(null, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1({}, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  function t2() {}
  t1(t2, {});
}

{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(1, []);
  t1('', []);
  t1(null, {});
  function t2() {}
  t1(t2, {});
}


{
  function t1(lhs, rhs) {
    let exp = lhs in rhs;
    let v = undefined;
    for (let i = 0; i < 50; i++) {
      v = lhs in rhs;
    }
    yes(v === exp);
  }
  t1(1, {});
  t1('', {});
  t1(null, {});
  t1(t2, {});
}
