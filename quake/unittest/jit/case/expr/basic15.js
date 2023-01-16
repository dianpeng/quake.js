{
  let i = 0;
  let out = 0;
  while (true) {
    out += i;
    i += 1;
    if (i >= 100) break;
  }
  yes(out == 4950);
  yes(i == 100);
}
