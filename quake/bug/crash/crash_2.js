


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
      return (
          Math.max(
              Math.sin(Math.log1p(Math.abs((+Math.atanh(Number.MIN_VALUE))))),
              Math.fround(
                  ((Math.pow(
                        Math.fround((y && (x ? 0 / 0 : x))),
                        Math.fround(
                            (Math.log((-Number.MIN_SAFE_INTEGER >>> 0)) >>> 0)))
                    << Math.asin(Math.cbrt(-(2 ** 53 + 2)))) >= (1 / 0, x)))) ?
              (~((Math.hypot(
                      (x >>> 0),
                      (+((y ? (((Math.acosh((x >>> 0)) >>> 0) | 0) ?
                                   ((Math.log1p(-0x080000000) | 0) | 0) :
                                   (y | 0)) :
                              42) >>>
                         0))) >>>
                  0) !== Math.fround((!(+y))))) :
              (Math.fround(Math.hypot(
                   (+-(2 ** 53 + 2)),
                   (+Math.cosh(
                       (!(((y | 0) ? (x | 0) : (+Math.clz32(x))) | 0)))))) >>
               ((+Math.fround((-Math.fround(y)))) <
                (((Math.fround(((x | 0) != Math.fround(x))) | 0), (x | 0)) |
                 0))));
    });
    testMathyFunction(mathy4, [
      1,
      0,
      -0,
      0 / 0,
      0x07fffffff,
      0x080000000,
      -0x07fffffff,
      -0x080000000,
      -0x080000001,
      0x0ffffffff,
      0x100000000,
      0x100000001,
      -0x100000000,
      -0x100000001,
      Number.MAX_VALUE,
      Number.MIN_SAFE_INTEGER,
      -Number.MIN_SAFE_INTEGER,
      -(2 ** 53),
      Number.MAX_SAFE_INTEGER,
      1.7976931348623157e308
    ]);
  }
})();

