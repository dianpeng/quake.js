{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    out &= 2;
  }
  yes(out == 2);
}

{
  let i = 0;
  let out = 2;
  let bit = 2;
  for (i = 0; i < 21; i++) {
    out &= bit;
  }
  yes(out == 2);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    out ^= 3;
  }
  yes(out == 1);
}

{
  let i = 0;
  let out = 2;
  let bit = 3;
  for (i = 0; i < 21; i++) {
    out ^= bit;
  }
  yes(out == 1);
}

