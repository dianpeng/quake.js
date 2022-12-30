function foo() {
  throw 100;
}


{
  let lhs_str = "a";
  let rhs_str = "b";
  let result = false;

  for (let i = 0; i < 1000; i++) {
    result = false || (lhs_str != rhs_str) || foo();
  }

  yes(result);
}
