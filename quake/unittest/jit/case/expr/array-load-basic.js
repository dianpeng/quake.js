{
  let arr = [1, 2, 3, 1+2+3];
  let result = 0;

  arr.a = 100;

  for (let i = 0; i < 1000; i++) {
    result = arr[0] + arr[1] + arr[2];
    result -= arr[3];

    arr[4] = 100;
    result -= arr[4];
    result += arr[4];

    // does not hurt named property (keyed)
    result += arr.a;
    result -= arr.a;
  }

  yes(result == 0);
}
