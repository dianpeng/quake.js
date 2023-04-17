{
  let result = [0];
  let undef = undefined;
  let nil = null;

  for (let i = 0; i < 10000; i++) {
    result[0] = undef != nil;  // false
    result[1] = undef == nil;  // true
    result[2] = undef < nil;   // false
    result[3] = undef > nil;   // false
    result[4] = undef <= nil;  // false
    result[5] = undef >= nil;  // false
  }

  no(result[0]);
  yes(result[1]);
  no(result[2]);
  no(result[3]);
  no(result[4]);
  no(result[5]);
}
