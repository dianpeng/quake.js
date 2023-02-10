{
  let arr0 = [0];
  let obj0 = {'a': 100};
  let array = [
    arr0,
    obj0,
    "abcd",
    "",
    null,
    undefined,
    true,
    100
  ];

  for (let i = 0; i < 1000; i++) {
    yes(array[0][0] == 0);
    yes(array[1].a == 100);
    yes(array[2] == "abcd");
    yes(array[3] == "");
    yes(array[4] === null);
    yes(array[5] === undefined);
    yes(array[6] === true);
    yes(array[7] === 100);
  }
}
