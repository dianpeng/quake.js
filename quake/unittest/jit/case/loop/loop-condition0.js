function gen_array(cnt) {
  let arr = [];
  for (let i = 0; i < cnt; i++) {
    arr[i] = 2;
  }
  return arr;
}

{
  const arr = gen_array(100);
  let sum = 0;
  for (let i = 0; i < arr.length; ++i) {
    sum += arr[i];
  }
  yes(sum == arr.length * 2);
}
