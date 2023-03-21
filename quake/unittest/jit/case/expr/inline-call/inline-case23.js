function safeAdd(x, y) {
  let lhs = x & 0xffff;
  let rhs = y & 0xffff;
  let lsw = lhs + rhs;
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

function entry() {
  let i;
  let olda;
  let oldb;
  let oldc;
  let oldd;
  let a = 1732584193;
  let b = -271733879;
  let c = -1732584194;
  let d = 271733878;
  let x = -1234554321;

  for (i = 0; i < 100000; i += 16) {
    olda = a;
    oldb = b;
    oldc = c;
    oldd = d;

    a = md5ff(a, b, c, d, x, 7, -680876936);
    d = md5ff(d, a, b, c, x, 12, -389564586);
    c = md5ff(c, d, a, b, x, 17, 606105819);
    b = md5ff(b, c, d, a, x, 22, -1044525330);
    a = md5ff(a, b, c, d, x, 7, -176418897);
    d = md5ff(d, a, b, c, x, 12, 1200080426);
    c = md5ff(c, d, a, b, x, 17, -1473231341);
    b = md5ff(b, c, d, a, x, 22, -45705983);
    a = md5ff(a, b, c, d, x, 7, 1770035416);
    d = md5ff(d, a, b, c, x, 12, -1958414417);
    c = md5ff(c, d, a, b, x, 17, -42063);
    b = md5ff(b, c, d, a, x, 22, -1990404162);
    a = md5ff(a, b, c, d, x, 7, 1804603682);
    d = md5ff(d, a, b, c, x, 12, -40341101);
    c = md5ff(c, d, a, b, x, 17, -1502002290);
    b = md5ff(b, c, d, a, x, 22, 1236535329);

    a = md5hh(a, b, c, d, x, 4, -378558);
    d = md5hh(d, a, b, c, x, 11, -2022574463);
    c = md5hh(c, d, a, b, x, 16, 1839030562);
    b = md5hh(b, c, d, a, x, 23, -35309556);
    a = md5hh(a, b, c, d, x, 4, -1530992060);
    d = md5hh(d, a, b, c, x, 11, 1272893353);
    c = md5hh(c, d, a, b, x, 16, -155497632);
    b = md5hh(b, c, d, a, x, 23, -1094730640);
    a = md5hh(a, b, c, d, x, 4, 681279174);
    d = md5hh(d, a, b, c, x, 11, -358537222);
    c = md5hh(c, d, a, b, x, 16, -722521979);
    b = md5hh(b, c, d, a, x, 23, 76029189);
    a = md5hh(a, b, c, d, x, 4, -640364487);
    d = md5hh(d, a, b, c, x, 11, -421815835);
    c = md5hh(c, d, a, b, x, 16, 530742520);
    b = md5hh(b, c, d, a, x, 23, -995338651);


    a = md5ii(a, b, c, d, x, 6, -198630844);
    d = md5ii(d, a, b, c, x, 10, 1126891415);
    c = md5ii(c, d, a, b, x, 15, -1416354905);
    b = md5ii(b, c, d, a, x, 21, -57434055);
    a = md5ii(a, b, c, d, x, 6, 1700485571);
    d = md5ii(d, a, b, c, x, 10, -1894986606);
    c = md5ii(c, d, a, b, x, 15, -1051523);
    b = md5ii(b, c, d, a, x, 21, -2054922799);
    a = md5ii(a, b, c, d, x, 6, 1873313359);
    d = md5ii(d, a, b, c, x, 10, -30611744);
    c = md5ii(c, d, a, b, x, 15, -1560198380);
    b = md5ii(b, c, d, a, x, 21, 1309151649);
    a = md5ii(a, b, c, d, x, 6, -145523070);
    d = md5ii(d, a, b, c, x, 10, -1120210379);
    c = md5ii(c, d, a, b, x, 15, 718787259);
    b = md5ii(b, c, d, a, x, 21, -343485551);

    a = safeAdd(a, olda);
    b = safeAdd(b, oldb);
    c = safeAdd(c, oldc);
    d = safeAdd(d, oldd);
  }
  return [a, b, c, d];
}

const value = entry();
yes(value[0] === 55164832);
yes(value[1] === -1179630817);
yes(value[2] === 1101439508);
yes(value[3] === -1001193870);
