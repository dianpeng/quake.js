{
  let sum = 0;
  let obj = new Object();
  for (let i = 0; i < 20; i++) {
    if (!obj) {
      sum = 1;
      break;
    }
  }
  yes(sum === 0);
}

{
  let sum = 0;
  let obj = [];
  for (let i = 0; i < 20; i++) {
    if (!obj) {
      sum = 1;
      break;
    }
  }
  yes(sum === 0);
}

{
  let sum = 0;
  let obj = [1];
  for (let i = 0; i < 20; i++) {
    if (!obj) {
      sum = 1;
      break;
    }
  }
  yes(sum === 0);
}

{
  let sum = 0;
  let obj = "a";
  for (let i = 0; i < 20; i++) {
    if (!obj) {
      sum = 1;
      break;
    }
  }
  yes(sum === 0);
}

{
  let sum = 0;
  let obj = "";
  for (let i = 0; i < 20; i++) {
    if (obj) {
      sum = 1;
      break;
    }
  }
  yes(sum === 0);
}
