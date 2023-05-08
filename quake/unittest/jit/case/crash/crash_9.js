


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


function oomAfterAllocations(...a) {}
function oomAtAllocation(...a) {}
function resetOOMFailure() {}

function relazifyFunctions(...a) {}

function disableSingleStepProfiling(...a) {}
function enableSingleStepProfiling(...a) {}

function bailout() {}
function bailAfter() {}
function getLcovInfo() {}
function deterministicgc() {}
function readGeckoProfilingStack() {}
function enableGeckoProfiling() {}
function enableGeckoProfilingWithSlowAssertions() {}
function disableGeckoProfiling(...a) {}
function baselineCompile() {}
function assertJitStackInvariants() {}
function setJitCompilerOption() {}
function enableOsiPointRegisterChecks() {}
function setIonCheckGraphCoherency(...a) {}
function fullcompartmentchecks() {}
function startgc() {}
function setGCCallback() {}
function allocationMarker() {}
function makeFinalizeObserver(...a) {}
function recomputeWrappers(...a) {}
function enableShapeConsistencyChecks(...a) {}
function grayRoot(...a) {}
function addMarkObservers(...a) {}
function clearMarkObservers(...a) {}
function getMarks(...a) {}
function createIsHTMLDDA() {
  return 100.0;
}
function gcparam() {}
function nukeAllCCWs() {}
function FakeDOMObject() {
  return {};
}
function isAsmJSCompilationAvailable() {
  return false;
}
function markObjectPropertiesUnknown(...a) {}


(() => {
  for (let kkkkk = 0; kkkkk < 20; kkkkk++) {
    mathy0 = (function(x, y) {
      return (
          ((+(Math.tanh(Math.fround(Math.fround((~Math.fround(x))))) !==
              (+((y + x) && (+(~x)))))) *
           (Math.hypot(
                (+Math.exp(Math.fround((+Math.fround(y))))),
                ((Math.pow((y >>> 0), (x >>> 0)) >>> 0) | 0)) |
            0)) !=
          Math.fround(
              (Math.fround((~Math.sign((x > (+(~y)))))) *
               Math.min(((+(y >>> 0)) >>> 0), (+y)))));
    });
    testMathyFunction(mathy0, [
      1, 0 / 0, 0x080000000, -0x07fffffff, -0x080000000, -0x080000001,
      -0x0ffffffff, -0x100000000, Number.MIN_VALUE, -Number.MAX_VALUE,
      Number.MIN_SAFE_INTEGER, -Number.MIN_SAFE_INTEGER, -(2 ** 53 - 2),
      -(2 ** 53 + 2), Number.MAX_SAFE_INTEGER, (2 ** 53) + 2,
      1.7976931348623157e308
    ]);
  }
})();

