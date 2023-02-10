{
  let result = [0];

  let num_lhs = 1.1;
  let num_rhs = 1.1;

  let bool_lhs = true;
  let bool_rhs = true;

  let str_lhs = "abcdefg";
  let str_rhs = "abcdefg";

  for (let i = 0; i < 1000; i++) {
    result[0] = (num_lhs == num_rhs);
    result[1] = (bool_lhs == bool_rhs);
    result[2] = (str_lhs == str_rhs);
  }

  yes(result[0]);
  yes(result[1]);
  yes(result[2]);
}
