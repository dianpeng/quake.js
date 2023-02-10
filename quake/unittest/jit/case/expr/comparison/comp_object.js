{
  let lhs = new Object();
  let rhs = new Object();
  let lhs_shadow = lhs;

  let result = [];

  for (let i = 0; i < 1000; i++) {
    result[0] = lhs == rhs;
    result[1] = lhs != rhs;
    result[2] = lhs <= rhs;
    result[3] = lhs < rhs;
    result[4] = lhs >= rhs;
    result[5] = lhs > rhs;
  }

  // WFT, JavaScript ?
  no(result[0]);
  yes(result[1]);
  yes(result[2]);
  no(result[3]);
  yes(result[4]);
  no(result[5]);
}
