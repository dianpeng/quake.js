{
  let obj = new Object();
  obj.counter = 0;

  let foo = () => {
    obj.a = ++obj.counter;
  };

  for (let i = 0; i < 1000; i++) {
    foo();
  }
  yes(obj.counter == 1000);
}
