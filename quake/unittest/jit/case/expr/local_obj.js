{
  let l0 = 100;
  for (let i = 0; i < 1000; i++) {
    l0 = new Object();
  }
  yes(typeof l0 == "object");
}
