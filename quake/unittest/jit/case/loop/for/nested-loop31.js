{
  let obj1 = {
    'a' : 10
  };
  let obj2 = {
    'b' : 9
  };

  let sum = 0;
  for (let i = 0; i < 20; i++) {
    for (let j = 0; j < 20; j++) {
      for (let k = 0; k < 20; k++) {
        sum += obj1.a - obj2.b;
      }
      sum -= obj2.b - obj1.a;
    }
  }

  yes(sum === 8400);
}
