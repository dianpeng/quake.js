{
  // strict comparison against undefined or null, ie testify null/undefined
  // reference pointer
  let a_number = 10;
  let a_bool = true;
  let a_null = null;
  let a_undefined = undefined;
  let a_object = new Object();
  let a_array = [];
  let a_function = () => { return 42; };

  for (let i = 0; i < 1000; ++i) {
    yes(a_number !== undefined);
    yes(a_bool !== undefined);
    yes(a_null !== undefined);
    yes(a_undefined === undefined);
    yes(a_object !== undefined);
    yes(a_array !== undefined);
    yes(a_function != undefined);
  }
}
