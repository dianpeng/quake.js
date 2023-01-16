{
	let i = 0;
	let out = 0;
	let boolvalue = true;

	for (i = 0; i < 10000; i++) {
		out += i;
		out -= i;
		out += boolvalue;
	}
	global_return = out;
}
