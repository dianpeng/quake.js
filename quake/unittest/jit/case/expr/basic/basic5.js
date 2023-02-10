{
  let i = 0;
  let out = 7;
  for (i = 0; i < 21; ++i) {
    out %= 5;
  }
  yes(out == 2);
}
