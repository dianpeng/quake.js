{
  let arr = [0, 0, 0, 0];
  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      for (let k = 0; k < 20; k++) {
        for (let z = 0; z < 20; z++) {
          arr[0]++;
        }
        arr[1]++;
      }
      arr[2]++;
    }
    arr[3]++;
  }

  yes(arr[0] == 20 * 20 * 20 * 20);
  yes(arr[1] == 20 * 20 * 20);
  yes(arr[2] == 20 * 20);
  yes(arr[3] == 20);
}
