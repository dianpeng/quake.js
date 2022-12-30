{
  let result = [];
  let undef = undefined;

  for (let i = 0; i < 1000; ++i) {
    result[0] = undef != undef;  // false
    result[1] = undef == undef;  // true
    result[2] = undef < undef;
    result[3] = undef <= undef;
    result[4] = undef > undef;
    result[5] = undef >= undef;
  }

  no(result[0]);
  yes(result[1]);
  no(result[2]);
  no(result[3]);
  no(result[4]);
  no(result[5]);
}
