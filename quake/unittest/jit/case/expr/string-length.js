{
  let result = [];
  let str0 = "";
  let str1 = "a";
  let str2 = "abc";

  for (let i = 0; i < 1000; ++i) {
    result[0] = str0.length;
    result[1] = str1.length;
    result[2] = str2.length;
  }

  yes(result[0] == 0);
  yes(result[1] == 1);
  yes(result[2] == 3);
}
