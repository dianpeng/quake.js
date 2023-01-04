{
  let obj = {};
  let foo = () => {
    obj.a = 100;
  };

  for (let i = 0; i < 1000; i++) {
    foo();
  }
  yes(obj.a === 100);
}
