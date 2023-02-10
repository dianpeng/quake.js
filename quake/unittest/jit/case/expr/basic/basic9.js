{
  let i = 0;
  let out = 2;
  for (i = 0; i < 16; i++) {
    out <<= 1;
  }
  yes(out == 131072);
}

{
  let i = 0;
  let out = 2;
  let bit = 1;
  for (i = 0; i < 16; i++) {
    out <<= bit;
  }
  yes(out == 131072);
}

{
  let i = 0;
  let out = 131072;
  let bit = 1;
  for (i = 0; i < 16; i++) {
    out >>= bit;
  }
  yes(out == 2);
}

{
  let i = 0;
  let out = 131072;
  for (i = 0; i < 16; i++) {
    out >>= 1;
  }
  yes(out == 2);
}
