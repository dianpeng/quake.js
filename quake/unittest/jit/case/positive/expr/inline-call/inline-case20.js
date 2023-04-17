{
  function safeAdd(x, y) {
    let lsw = (x & 0xffff) + (y & 0xffff);
    let msw = (x >> 16) + (y >> 16) + (lsw >> 16);
    return (msw << 16) | (lsw & 0xffff);
  }
  function bitRotateLeft(num, cnt) {
    return (num << cnt) | (num >>> (32 - cnt));
  }
  function md5cmn(q, a, b, x, s, t) {
    return safeAdd(bitRotateLeft(safeAdd(safeAdd(a, q), safeAdd(x, t)), s), b);
  }
  function md5ff(a, b, c, d, x, s, t) {
    return md5cmn((b & c) | (~b & d), a, b, x, s, t);
  }
  function md5gg(a, b, c, d, x, s, t) {
    return md5cmn((b & d) | (c & ~d), a, b, x, s, t);
  }
  function md5hh(a, b, c, d, x, s, t) {
    return md5cmn(b ^ c ^ d, a, b, x, s, t);
  }
  function md5ii(a, b, c, d, x, s, t) {
    return md5cmn(c ^ (b | ~d), a, b, x, s, t);
  }

  function entry(s) {
    let sum  = 0;
    for (let i = 0; i < 1000; i++) {
      sum += md5ff(1, 2, 3, -1, -2, -3, 12345677);
      sum += md5gg(1, 2, 3, -1, -2, -3, 2234567);
      sum += md5hh(1, 2, 3, -1, -2, -3, 33333444);
      sum += md5ii(1, 2, 3, -1, -2, -3, 22244433);
    }
    return sum;
  }

  function gen(length) {
    return 'a'.repeat(length);
  }

  const value = entry(gen(1024 * 128));
  yes(value === 2693124332000);
}
