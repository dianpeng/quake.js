{
  let i = 0;
  let out = 2;
  for (i = 0; i < 16; i++) {
    if (i >= 14) out **= 2;
  }
  yes(out == 16);
}
