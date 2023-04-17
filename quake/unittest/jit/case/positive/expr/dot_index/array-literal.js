{
  for (let i = 0; i < 15; i++) {
    yes([].length == 0);
    yes([1].length == 1);
		yes(Object.entries({}).length == 0);
		yes(Object.entries({'a':100}).length == 1);

    yes([
      0,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      9,
      10,
      42][11] == 42);
  }
}
