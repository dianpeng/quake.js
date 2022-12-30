{
  let result = 0;
  let lhs = 1000.1;
  let rhs = 1000.100001;

  for (let i = 0; i < 1000; i++) {
    result = lhs < rhs;
  }
  yes(result);
}
