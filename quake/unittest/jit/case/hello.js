{
  let i = 0;
  let result = 0;

  for (i = 0; i < 100; i++) {
    result = i;
  }

  dump("Welcome to the Quake.js's JIT testing suits");
  yes(result == 99);
}
