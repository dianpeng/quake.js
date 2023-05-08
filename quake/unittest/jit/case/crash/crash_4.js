


function print(x) {
  dump(x);
}

function schedulegc(...a) {}
function schedulezone(...a) {}
function selectforgc(...a) {}
function gczeal(...a) {}
function gc(...a) {}
function gcslice(...a) {}
function verifyprebarriers(...a) {}
function gcPreserveCode(...a) {}

function dumpln(x) {
  dump(x);
}

function testMathyFunction(f, inputs) {
  var results = [];
  if (f) {
    for (var j = 0; j < inputs.length; ++j) {
      for (var k = 0; k < inputs.length; ++k) {
        results.push(f(inputs[j], inputs[k]));
      }
    }
  }
}

function hashStr(s) { /* eslint-disable-line require-jsdoc */
  var hash = 0;
  var L = s.length;
  for (var i = 0; i < L; i++) {
    var c = s.charCodeAt(i);
    hash = (Math.imul(hash, 31) + c) | 0;
  }
  return hash;
}


(() => {
  for (let kkkkk = 0; kkkkk < 20; kkkkk++) {
    mathy4 = (function(x, y) {
      return (~Math.sign((
          Math.cosh((+Math.fround(Math.pow(x, (((~y) >>> 0) ** (y != y)))))) >>>
          0)));
    });
    testMathyFunction(mathy4, [
      Math.PI,
      0,
      0 / 0,
      1 / 0,
      -1 / 0,
      0x080000000,
      0x080000001,
      -0x07fffffff,
      -0x080000001,
      0x0ffffffff,
      0x100000000,
      0x100000001,
      -0x0ffffffff,
      -0x100000000,
      Number.MIN_VALUE,
      -Number.MIN_VALUE,
      -Number.MIN_SAFE_INTEGER,
      -(2 ** 53 - 2),
      -(2 ** 53),
      Number.MAX_SAFE_INTEGER,
      (2 ** 53),
      0.000000000000001
    ]);
  }
})();

