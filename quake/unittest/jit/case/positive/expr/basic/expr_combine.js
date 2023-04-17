{
  let i = 0;
  let result = 0;
  let t0, t1, t2, t3, t4;

  for (i = 0; i < 10000; ++i) {
    t0 = i * 2;
    t1 = t0 << 1;
    t2 = t1 + 128;
    t3 = t2 >> 2;
    t4 = t3 % 7;
    result += t4;
  }

  yes(result == 30003);
}
