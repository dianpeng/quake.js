{
  let result = "";
  let array = ["a", "b", "c", "d"];

  for (let i = 0; i < 1000; i++) {
    result = "";
    result += array[0];
    result += array[1];
    result += array[2];
    result += array[3];
  }
  yes(result === "abcd");
}
