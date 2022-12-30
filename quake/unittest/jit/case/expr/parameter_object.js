function foo(a, b) {
  let o1 = new Object();
  o1.a = 100;

  let o2 = new Object();
  o2.b = 200;

  for (let i = 0; i < 1000; i++) {
    a = o1;
    b = o2;
  }
  return a.a + b.b;
}

yes(foo({'u': 100}, {'v': 200}) == 300);
