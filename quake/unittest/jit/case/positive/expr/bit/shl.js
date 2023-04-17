/**
 * Shl internally will/can overflow the 32bits range, so we need to test its
 * overflow behavior
 */

{
  let v = 0;
  let lhs = 2;
  let rhs = 10;
  for (let i = 0; i < 20; i++) {
    v = lhs << rhs;
  }
  yes(v === 2 << 10);
}

{
  let v = 0;
  let lhs = 2;
  for (let i = 0; i < 20; i++) {
    v = lhs << 10;
  }
  yes(v === 2 << 10);
}

{
  let v = 0;
  let lhs = 2;
  let rhs = 102;
  for (let i = 0; i < 20; i++) {
    v = lhs << rhs;
  }
  yes(v === 128);
}

{
  let v = 0;
  let lhs = 2;
  let rhs = 102;
  for (let i = 0; i < 20; i++) {
    v = lhs << 102;
  }
  yes(v === 128);
}

{
  let v = 0;
  let lhs = 2;
  let rhs = -1024102;
  for (let i = 0; i < 20; i++) {
    v = lhs << rhs;
  }
  yes(v === 134217728);
}

{
  let v = 0;
  let lhs = 2;
  for (let i = 0; i < 20; i++) {
    v = lhs << -1024102;
  }
  yes(v === 134217728);
}

{
  let v = 0
  let lhs = 1;
  let rhs = 31;
  for (let i = 0; i < 20; i++) {
    v = lhs << rhs;
  }
  yes(v === -2147483648);
}

/**
 * Overflow testing cases
 */

{
  let v = 0;
  let lhs = 4096;
  for (let i = 0; i < 20; i++) {
    lhs = lhs << 2;
  }
  yes(lhs === 0);
}

{
  let v = 0;
  let lhs = -1024;
  for (let i = 0; i < 40; i++) {
    lhs = lhs << 2;
  }
  yes(lhs === 0);
}
