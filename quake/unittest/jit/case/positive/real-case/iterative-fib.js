iterative_function = function(n) {
  let sequenceArray = [0, 1];
  for (let i = 2; i <= n; i++) {
    sequenceArray.push(sequenceArray[i - 1] + sequenceArray[i - 2]);
  }
  return sequenceArray[sequenceArray.length-1];
}

{
  const value = iterative_function(100);
  yes(value == 354224848179262000000);
}
