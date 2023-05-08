


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
    mathy4 = (function(x, y) {
      return (
          Math.fround(
              (Math.fround(
                   (Math.fround(
                        (((+(Math.imul(
                                 (Math.log10(y) | 0),
                                 (+Math.max(
                                     1.7976931348623157e308, 0x100000001))) |
                             0)) &&
                          ((-y) | 0)) |
                         0)) ?
                        Math.fround((((x | 0) || Math.fround(x)) | 0)) :
                        (+Math.fround(Math.hypot(x, (~y)))))) ?
                   Math.fround(
                       (Math.fround(y) ?
                            Math.fround(Number.MAX_SAFE_INTEGER) :
                            ((x >>> 0) *
                             ((Math.fround(Math.tan(Number.MAX_VALUE)), x) >>>
                              0)))) :
                   Math.min(
                       Math.log1p((~((+(Math.acos(y) | 0)) | 0))),
                       (!(+(~(y - y))))))) ==
          (-Math.fround((Math.tan((+(!x))) || Math.imul((+(y >> (~-0))), 0)))));
    });
    testMathyFunction(mathy4, [
      0 / 0, 0x080000000, 0x080000001, -0x080000000, 0x100000000, -0x100000000,
      -Number.MIN_VALUE, Number.MAX_VALUE, -Number.MAX_SAFE_INTEGER,
      0.000000000000001, 1.7976931348623157e308
    ]);
  }
})();

