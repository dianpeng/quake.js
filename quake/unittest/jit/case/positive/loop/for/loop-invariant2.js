{
  let a = "abcdef";
  let result = 0;
  let that = "xxx";

  for (let i = 0; i < 1000; i++) {
    result = a.indexOf(that);
  }
  yes(result === -1);
}
