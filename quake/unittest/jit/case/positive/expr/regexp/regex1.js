{
  let result = 0;
  let rhs = "abbbc";
  for (let i = 0; i < 1000; i++) {
    let pattern = /ab+c/;
    result = rhs.match(pattern);
  }
  yes(result);
}
