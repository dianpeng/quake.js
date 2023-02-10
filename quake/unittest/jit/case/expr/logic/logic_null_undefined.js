{
  let nil = null;
  let undef = undefined;
  let result = false;

  for (let i = 0; i < 1000; i++) {
    result = true && undef;
    no(result);

    result = false || nil;
    no(result);

  }
}
