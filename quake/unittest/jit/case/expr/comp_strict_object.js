{
  let lhs = new Object();
  let rhs = [];
  let lhs_shadow = lhs;

  let result = [];

  for (let i = 0; i < 1000; i++) {
    result[0] = lhs === rhs;
    result[1] = lhs !== rhs;
    result[2] = lhs == lhs_shadow;
    result[3] = lhs !== lhs_shadow;
  }

  no(result[0]);
  yes(result[1]);
  yes(result[2]);
  no(result[3]);
}
