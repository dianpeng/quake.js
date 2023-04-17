{

  function b(v) {
    return v + 10;
  }

  function a(len) {
    const arr = [1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5];
    let sum = 0;
    const v = arr[len];
    sum += b(v);
    return sum;
  }

  let ttt = 0;

  for (let i = 0; i < 20; i++) {
    ttt = a(i);
    dump(`iter: ${i}`);
  }

  dump(ttt);
}
