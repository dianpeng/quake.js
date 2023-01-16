{
  let out = 0;
  for (let i = 0; i < 21; i++) {
    out += 3;
    out *= 2;
    out -= 3;
    out /= 3;
    yes(out == 1);
    out = 0;
  }
}
