{
  let i = 0;
  let result = 0;
  let obj = new Object();

  obj.a = 10;
  obj.b = 20;
  obj.c = 30;
  obj.d = 40;
  obj.e = 50;

  for (i = 0; i < 10000; ++i) {
    let tmp = obj['a'] + obj['b'] + obj['c'] + obj['d'] + obj['e'];

    result = tmp;
    result -= tmp;

    obj.z = 100;
    result += obj.z;
    result -= obj.z;

    obj.vv = 100;
    delete obj.vv;
    let u = obj.vv; // fallback to slow path

    result = u === undefined;
  }

  yes(result == 1);
}
