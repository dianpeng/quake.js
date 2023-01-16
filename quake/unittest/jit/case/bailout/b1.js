{
	let i = 0;
	let out = -2147483636;
	let value = 0;
	for (i = 0; i < 15; i++) {
		value = --out;
	}
  dump(value);
  yes(value == -2147483651.0);
}
