{
  let i = 0;
  let out = 0;
  for (i = 0; i < 100; i++) {
    out += i;
  }
  yes(i == 100);
  yes(out == 4950);
}
