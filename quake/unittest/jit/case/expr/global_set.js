{
  global_1 = 100;
  for (let i = 0; i < 1000; i++) {
    global_1 = i;
  }

  yes(global_1 == 999);
}
