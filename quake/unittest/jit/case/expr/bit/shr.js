/**
 * ----------------------------------------------------------------------------
 * SAR represents a >>> b operation in quickjs.
 *
 * The lhs and rhs of SAR operation will be considered as unsigned integer
 * ----------------------------------------------------------------------------
 */

{
  let v = 0;
  let lhs = 2147483646;
  let rhs = 30;

  for (let i = 0; i < 30; i++) {
    v = lhs >>> rhs;
  }
  yes(v === 1);
}

{
  let v = 0;
  let lhs = 21474836476;
  let rhs = 31;
  for (let i = 0; i < 30; i++) {
    v = lhs >>> rhs;
  }
  yes(v === 1);
}

{
  let v = 0;
  let lhs = -2147483648;
  let rhs = 20;
  for (let i = 0; i < 30; i++) {
    v = lhs >>> rhs;
  }
  yes(v === 2048);
}
