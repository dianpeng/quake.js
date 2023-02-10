{
  const a = /ab+c/;
  const rhs = "abbbbc";
  let result = false;
  for (let i= 0 ; i < 100; i++) {
    result = rhs.match(a);
  }
  yes(result);
}
