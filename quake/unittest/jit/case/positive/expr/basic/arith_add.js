{
  for (let i = 0; i < 1000; i++) {
    yes(1 + 1 == 2);
    yes(1 + true == 2);
    yes(true + false == 1);
    yes(null + null == 0);
    yes(null + true == 1);
  }
}
