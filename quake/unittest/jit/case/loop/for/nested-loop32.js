{
  let foo = function(a) { return a; }
  let bar = function(a, b) { return a.a - b.b; }
  let o1 = {'a': 10};
  let o2 = {'b': 8 };
  let sum = 0;
  for (let i = 0; i < 20; i++) {
    for (let k = 0; k < 20; k++) {
      for (let j = 0; j < 20; j++) {
        for (let z = 0; z < 20; z++) {
          sum += bar(o1, o2) - foo(1);
        }
      }
    }
  }
  yes(sum === 20*20*20*20);
}
