{
  let i = 0;
  let out = 2;
  for (i = 0; i < 20; i++) {
    out *= 2;
  }
  yes(out == 2097152);
}
