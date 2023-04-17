{
  let l0, l1, l2, l3, l4;

  for (let i = 0; i < 100; i++) {
    l0 = i - 4;
    l1 = i - 3;
    l2 = i - 2;
    l3 = i - 1;
    l4 = i - 0;
  }

  yes(l0 == 99-4);
  yes(l1 == 99-3);
  yes(l2 == 99-2);
  yes(l3 == 99-1);
  yes(l4 == 99-0);
}
