{
  let str_123 = "123";
  let num_123 = 123;
  let str_1_1 = "1.1";
  let num_1_1 = 1.1;

  let str_nan = "abcd";

  let result = [];
  for (let i = 0; i < 1000; i++) {
    result[0] = str_123 == num_123;
    result[1] = str_1_1 == num_1_1;
    result[2] = str_nan == num_123;
    result[3] = str_nan == num_1_1;
    result[4] = str_nan == NaN;
  }

  yes(result[0]);
  yes(result[1]);

  no(result[2]);
  no(result[3]);
  no(result[4]);
}
