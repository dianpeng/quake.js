{
  let i = 0;
  let result = 0;
  let obj = new Object();
  let tmp = 0;

  obj.a = 10;
  obj.b = 20;
  obj.c = 30;
  obj.d = 40;
  obj.e = 50;

  for (i = 0; i < 10000; ++i) {
    obj.z = 100;
    tmp = obj.a + obj.b + obj.c + obj.d + obj.e;

    result = tmp;
    result -= tmp;

    result += obj.z;
    result -= obj.z;

    obj.vv = 1000;
    delete obj.vv;
  }

  yes(result == 0);
}
