


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
    mathy5 = (function(x, y) {
      return (
          Math.trunc(
              (Math.cosh(((Math.tan(((~(y | 0)) | 0)) >>> 0) >>> 0)) >>> 0)) >=
          Math.fround(Math.atan2(
              Math.fround(Math.fround(Math.atan2(
                  Math.fround(
                      ((Math.fround(Math.log2(Math.fround(x))) & y) !=
                       (Math.sign(y) >>> 0))),
                  Math.fround(Math.tan(Math.fround((+(y <= x)))))))),
              Math.fround(
                  ((~(Math.hypot(
                          (+y),
                          (Math.hypot(
                               Math.fround((!y)),
                               Math.fround(Math.tanh((x >>> 0)))) >>>
                           0)) >>>
                      0)) >>>
                   0)))));
    });
    testMathyFunction(mathy5, [
      1, -0, 0 / 0, 1 / 0, -1 / 0, 0x07fffffff, 0x080000001, -0x0ffffffff,
      -0x100000001, -Number.MIN_VALUE, Number.MAX_VALUE, -Number.MAX_VALUE,
      -(2 ** 53 - 2), -(2 ** 53), (2 ** 53) - 2, (2 ** 53)
    ]);
  }
})();

