{
  let result = 0;
  let lhs_str = "a";
  let rhs_str = "b";

  let foo = () => {
    throw 100;
  };

  for (let i = 0; i < 20; i++) {
    result = 1               &&
          true               &&
          lhs_str == rhs_str && /* shortcut */
          foo() == 42;
  }
  no(result);
}
