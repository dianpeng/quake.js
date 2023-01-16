{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; ++i) {
    out = -out;
  }
  yes(out == -2);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; ++i) {
    out = +out;
  }
  yes(out == 2);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; ++i) {
    out = !out;
  }
  yes(!out);
}

