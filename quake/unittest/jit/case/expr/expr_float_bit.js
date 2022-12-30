{
  // bit operation on floating point. The floating point number needs to be
  // casted into integer implicitly. The routine is hardcoded as inline assembly
  // inside quake based on qucick.js C code, which may or may not be correct :)
  
  let lhs = 63.444;
  let rhs = 2.1;
  let result = [];

  for (let i = 0; i < 100; i++) {
    result[0] = lhs << rhs;
    result[1] = lhs >> rhs;
    result[2] = lhs & rhs;
    result[3] = lhs | rhs;
    result[4] = lhs ^ rhs;
  }

  yes(result[0] == 252);
  yes(result[1] == 15);
  yes(result[2] == 2);
  yes(result[3] == 63);
  yes(result[4] == 61);
}
