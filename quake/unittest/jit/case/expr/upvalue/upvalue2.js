{
  let a = 100;
  let foo = () => {
    a = 1000;
  };

  for (let i = 0; i < 1000; i++) {
    foo();
  }
  yes(a === 1000);
}
