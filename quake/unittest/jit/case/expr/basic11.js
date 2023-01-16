{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    out++;
  }
  yes(out == 23);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    out--;
  }
  yes(out == -19);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    ++out;
  }
  yes(out == 23);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 21; i++) {
    --out;
  }
  yes(out == -19);
}

{
  let i = 0;
  let out = 2;
  for (i = 0; i < 20; i++) {
    if (i >= 19) {
      yes(out-- == 2);
      yes(out == 1);
      yes(--out == 0);
      yes(out == 0);

      yes(out++ == 0);
      yes(out == 1);
      yes(++out == 2);
    }
  }
  yes(out == 2);
}
