{
  let lhs = '';
  let rhs = '';
  let result = [];


  for (let i = 0; i < 1000; i++) {
    result[0] = lhs <= rhs;
    result[1] = lhs < rhs;
    result[2] = lhs == rhs;
    result[3] = lhs > rhs;
    result[4] = lhs >= rhs;
    result[5] = lhs != rhs;
  }

  yes(result[0]);
  no(result[1]);
  yes(result[2]);
  no(result[3]);
  yes(result[4]);
  no(result[5]);
}
