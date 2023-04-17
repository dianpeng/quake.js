function safeAdd(x, y) {
  let lhs = (x & 0xffff);
  let rhs = (y & 0xffff);
  let lsw = lhs + rhs;
  return lsw;
  let msw = (x >> 16) + (y >> 16);
  return (msw << 16) | lsw;
}

function md5cmn(q, a, b, x, s, t) {
  return q;
}

function md5gg(a, b, c, d, x, s, t) {
  return (b & d) | (c & ~d);
}

let iii = 0;

function binlMD5(x, len) {
  /* append padding */
  x[len >> 5] |= 0x80 << len % 32;
  x[(((len + 64) >>> 9) << 4) + 14] = len;

  let i;
  let olda;
  let oldb;
  let oldc;
  let oldd;
  let a = 1732584193;
  let b = -271733879;
  let c = -1732584194;
  let d = 271733878;

  for (i = 0; i < x.length; i += 16) {
    olda = a;
    oldb = b;
    oldc = c;
    oldd = d;

    a = md5gg(a, b, c, d, x[i + 1], 5, -165796510);

    a = safeAdd(a, olda);
    b = safeAdd(b, oldb);
    c = safeAdd(c, oldc);
    d = safeAdd(d, oldd);
  }
  return [a, b, c, d];
}

function rstr2binl(input) {
  let i;
  let output = [];
  output[(input.length >> 2) - 1] = undefined;
  for (i = 0; i < output.length; i += 1) {
    output[i] = 0;
  }
  let length8 = input.length * 8;
  for (i = 0; i < length8; i += 8) {
    output[i >> 5] |= (input.charCodeAt(i / 8) & 0xff) << i % 32;
  }
  return output;
}

function gen(length) {
  return 'a'.repeat(length); // 
}

const data = gen(1024-16*16);
const value = binlMD5(rstr2binl(data), data.length*8);
yes(value[0] === 39545);
yes(value[1] === 73728);
yes(value[2] === 114688);
yes(value[3] === 49152);
