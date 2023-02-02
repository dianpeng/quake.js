function genArray(length) {
  return new Array(length).fill(1);
}

for (let x of genArray(100)) {}
