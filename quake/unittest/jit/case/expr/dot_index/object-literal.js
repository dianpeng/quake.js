{
  let result = 0;
  for (let i = 0; i < 100; i++) {
    result = {'x': 42}.x;
    yes(result == 42);
    yes(
      {
        'a': 1,
        'b': 2,
        'c': 3,
        'd': 4,
        'e': 5,
        'f': 6,
        'z': 42
      }.z == 42
    );
  }
}
