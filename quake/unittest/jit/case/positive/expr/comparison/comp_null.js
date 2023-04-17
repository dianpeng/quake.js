{
  let nil = null;
  let result = [];

  for (let i = 0; i < 1000; i++) {
    result[0] = nil == nil;
    result[1] = nil != nil;
    result[2] = nil <= nil;
    result[3] = nil < nil;
    result[4] = nil >= nil;
    result[5] = nil > nil;
  }

  yes(result[0]);
  no(result[1]);

  yes(result[2]);
  no(result[3]);

  yes(result[4]);
  no(result[5]);
}
