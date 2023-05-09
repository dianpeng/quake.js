{
  function Thing(is_new) {
    if (is_new) {
      yes(new.target !== undefined);
    } else {
      yes(new.target === undefined);
    }
  }

  let v = undefined;
  for (let i = 0; i < 1; i++) {
    v = new Thing(true);
  }
}

{
  function Thing(is_new) {
    if (is_new) {
      yes(new.target !== undefined);
    } else {
      yes(new.target === undefined);
    }
  }

  let v = undefined;
  for (let i = 0; i < 1; i++) {
    v = Thing(false);
  }
}
