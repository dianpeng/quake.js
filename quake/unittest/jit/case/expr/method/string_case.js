{
  let lower = "abc";
  let upper = "ABC"; 

  let result = [];
  for (let i = 0; i < 100; i++) {
    result[0] = lower.toUpperCase();
    result[1] = upper.toLowerCase();
  }

  yes(result[0] == upper);
  yes(result[1] == lower);
}
