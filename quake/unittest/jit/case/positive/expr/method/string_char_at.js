{
  let string = "hello";
  let result1 = 0;
  let result2 = 0;

  for (let i = 0; i < 10000; ++i) {
    result1 = string.charAt(4);
    result2 = "hello".charAt(4);
  }

  yes(result1 === 'o');
  yes(result2 === 'o');
}
