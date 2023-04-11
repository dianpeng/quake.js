/**
 * (shr) operation in quickjs is been modeled as following
 *
 * A *signed* shift on a *unsigned* of second operands. Notes the second operand
 * will be clamped into 8 bits, by and 0x1f.
 */

{
  let v = 0;
  let lhs = 10;
  let rhs = 2;
  for (let i = 0; i < 20; i++) {
    v = lhs >> rhs;
  }
  yes(10 >> 2 === v);
}

{
  let v = 0;
  let lhs = 10;
  for (let i = 0; i < 20; i++) {
    v = lhs >> 2;
  }
  yes(10 >> 2 === v);
}

{
  let v = 0;
  let lhs = 1024 * 1024;
  let rhs = 1111122;

  for (let i = 0; i < 20; i++) {
    v = lhs >> rhs;
  }
  yes((1024 * 1024) >> 18 === v);
}

{
  let v = 0;
  let lhs = 1024 * 1024;

  for (let i = 0; i < 20; i++) {
    v = lhs >> 1111122;
  }
  yes((1024 * 1024) >> 18 === v);
}

{
  let v = 0;
  let lhs = 1024 * 1024;
  for (let i = 0; i < 20; i++) {
    v = lhs >> -10241024;
  }
  yes(1048576 === v);
}

{
  let v = 0;
  let lhs = 1024 * 1024;
  let rhs = -10241024;
  for (let i = 0; i < 20; i++) {
    v = lhs >> rhs;
  }
  yes(1048576 === v);
}
