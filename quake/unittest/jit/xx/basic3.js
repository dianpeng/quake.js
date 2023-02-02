{
  let sum1 = 0;   // 1
  let sum2 = 0;   // 2
  let sum3 = 0;   // 3
  let value = 0;  // 4

  // i 5
  // j 6
  // k 7

  jit_trace_on();

  for (let i = 0; i < 20; ++i) {
    for (let j = 0; j < 20; j++) {
      for (let k = 0; k < 20; k++) {
        ++sum1;
      }
      ++sum2;
    }
    ++sum3;
  }

  yes(sum1 == 8000);
  yes(sum2 == 400);
  yes(sum3 == 20);
}
