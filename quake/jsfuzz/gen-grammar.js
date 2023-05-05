
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/* exported linkedList, makeAsmJSFunction, makeAsmJSModule, makeCrazyToken,
 * maybeMakeTerm, strTimes */
/* global allMethodNames, allPropertyNames, arrayBufferType, asmJSInterior,
 * binaryMathFunctions, builtinFunctions */
/* global builtinObjectNames, builtinProperties, cat, constructors, engine,
 * ENGINE_JAVASCRIPTCORE, evalcx */
/* global fuzzTestingFunctionsCtor, jsshell, js_src_tests_dir, libdir,
 * loopCount, loopModulo, makeBuilderStatement */
/* global makeEvilCallback, makeImmediateRecursiveCall, makeMathExpr,
 * makeMathFunction, makeMathyFunAndTest */
/* global makeMathyFunRef, makeProxyHandler, makeProxyHandlerFactory, makeRegex,
 * makeRegexUseBlock, makeRegexUseExpr */
/* global makeRegisterStompBody, oomTest, Random, read, regressionTestList,
 * regressionTestsRoot, rnd, simpleSource */
/* global stripSemicolon, TOTALLY_RANDOM, totallyRandom, unaryMathFunctions,
 * uneval, UNTERMINATED_COMMENT */
/* global w_pltfrm_res_dir, XPCNativeWrapper, xpcshell */

/* ************************ *
 * GRAMMAR-BASED GENERATION *
 * ************************ */

const RND = require('./rnd.js');
const rnd = RND.rnd;
const Random = RND.Random;

function dumpln(xxx) {
  // console.error('[dumpln]: ' + xxx);
}

function fatal(xxx) {}

var UNTERMINATED_COMMENT =
    '/*'; /* this comment is here so my text editor won't get confused */
var UNTERMINATED_STRING_LITERAL = '\'';
var UNTERMINATED_REGEXP_LITERAL = '/';

function maybeLineBreak() { /* eslint-disable-line require-jsdoc */
  if (rnd(900) === 3) {  // line break to trigger semicolon insertion and stuff
    return Random.index(['\r', '\n', '//h\n', '/*\n*/']);
  } else if (rnd(400) === 3) {  // weird space-like characters
    return rnd(2) ? '\u000C' : '\t';
  } else {
    return '';
  }
}

function maybeSpace() { /* eslint-disable-line require-jsdoc */
  if (rnd(2) === 0) {
    return ' ';
  } else {
    return '';
  }
}

function stripSemicolon(c) { /* eslint-disable-line require-jsdoc */
  var len = c.length;
  if (c.charAt(len - 1) === ';') {
    return c.substr(0, len - 1);
  } else {
    return c;
  }
}


function confused(s) { /* eslint-disable-line require-jsdoc */
  if (jsshell) {
    // Magic string that js_interesting looks for
    // Currently disabled until its use can be figured out
    // print("jsfunfuzz broke" + " its own scripting environment: " + s);
    // Replaced with the following:
    print(`jsfunfuzz got confused: ${s}`);
    quit();
  }
}

function foundABug(summary, details) { /* eslint-disable-line require-jsdoc */
  // Magic pair of strings that js_interesting looks for
  // Break up the following string so internal js functions do not print it
  // deliberately
  let foundMsg = `Found`;
  foundMsg += ` a bug: ${summary}`;
  printImportant(foundMsg);
  if (details) {
    printImportant(details);
  }
  if (jsshell) {
    dumpln('jsfunfuzz stopping due to finding a bug.');
    quit();
  }
}

function errorToString(e) { /* eslint-disable-line require-jsdoc */
  try {
    return (`${e}`);
  } catch (e2) {
    return 'Can\'t toString the error!!';
  }
}

function errorstack() { /* eslint-disable-line require-jsdoc */
  print('EEE');
  try {
    void ([].qwerty.qwerty);
  } catch (e) {
    print(e.stack);
  }
}


var jsStrictMode = false;
var constructors = [];      // "Array"
var builtinFunctions = [];  // "Array.prototype.sort"
var builtinProperties =
    [];  // "Array", "Array.prototype", "Array.prototype.length"
var allMethodNames = [];    // "sort"
var allPropertyNames = [];  // "length"

var builtinObjectNames =
    [];  // "Array", "Array.prototype", ... (indexes into the builtinObjects)
var builtinObjects = {};  // { "Array.prototype": ["sort", "length", ...], ... }

(function exploreBuiltins(glob, debugMode) {
  function exploreDeeper(a, an) { /* eslint-disable-line require-jsdoc */
    if (!a) {
      return;
    }
    var hns = Object.getOwnPropertyNames(a);
    var propertyNames = [];
    for (var j = 0; j < hns.length; ++j) {
      var hn = hns[j];
      propertyNames.push(hn);
      allPropertyNames.push(hn);

      var fullName = `${an}.${hn}`;
      builtinProperties.push(fullName);

      var h;
      try {
        h = a[hn];
      } catch (e) {
        if (debugMode) {
          dumpln(`Threw: ${fullName}`);
        }
        h = null;
      }

      if (typeof h === 'function' && hn !== 'constructor') {
        allMethodNames.push(hn);
        builtinFunctions.push(fullName);
      }
    }
    builtinObjects[an] = propertyNames;
    builtinObjectNames.push(an);
  }

  function exploreConstructors() { /* eslint-disable-line require-jsdoc */
    var gns = Object.getOwnPropertyNames(glob);
    for (var i = 0; i < gns.length; ++i) {
      var gn = gns[i];
      // Assume that most uppercase names are constructors.
      // Skip Worker in shell (removed in bug 771281).
      if (gn.charCodeAt(0) > 0x40 && gn.charCodeAt(0) < 0x60 &&
          gn !== 'PerfMeasurement' && !(jsshell && gn === 'Worker')) {
        var g = glob[gn];
        if (typeof g === 'function' &&
            g.toString().indexOf('[native code]') !== -1) {
          constructors.push(gn);
          builtinProperties.push(gn);
          builtinFunctions.push(gn);
          exploreDeeper(g, gn);
          exploreDeeper(g.prototype, `${gn}.prototype`);
        }
      }
    }
  }

  exploreConstructors();

  exploreDeeper(Math, 'Math');
  exploreDeeper(JSON, 'JSON');
  exploreDeeper(Proxy, 'Proxy');

  if (debugMode) {
    for (let x of constructors) print(`^^^^^ ${x}`);
    for (let x of builtinProperties) print(`***** ${x}`);
    for (let x of builtinFunctions) print(`===== ${x}`);
    for (let x of allMethodNames) print(`!!!!! ${x}`);
    for (let x of allPropertyNames) print(`&&&&& ${x}`);
    print(uneval(builtinObjects));
    quit();
  }
})(this, false);


/* exported gcIsQuiet, loopCount, loopModulo, readline, xpcshell */
/* global debug, gc, print, readline:writable, rnd, uneval:writable,
 * verifyprebarriers, wasmIsSupported */
/* XPCNativeWrapper */

// jsfunfuzz is best run in a command-line shell.  It can also run in
// a web browser, but you might have trouble reproducing bugs that way.

var ENGINE_UNKNOWN = 0;
var ENGINE_SPIDERMONKEY_TRUNK = 1;
var ENGINE_JAVASCRIPTCORE = 4;

var engine = ENGINE_UNKNOWN;
var jsshell =
    (typeof window === 'undefined'); /* eslint-disable-line no-undef */
var xpcshell = jsshell &&
    (typeof Components === 'object'); /* eslint-disable-line no-undef */
var dumpln;
var printImportant;

printImportant = function(s) {
  dumpln('***');
  dumpln(s);
};

if (typeof verifyprebarriers === 'function') {
  // Run a diff between the help() outputs of different js shells.
  // Make sure the function to look out for is not located only in some
  // particular #ifdef, e.g. JS_GC_ZEAL, or controlled by --fuzzing-safe.
  if (typeof wasmIsSupported === 'function') {
    engine = ENGINE_SPIDERMONKEY_TRUNK;
  }

  // Avoid accidentally waiting for user input that will never come.
  readline = function() {};
} else if (typeof XPCNativeWrapper === 'function') { /* eslint-disable-line
                                                        no-undef */
  // e.g. xpcshell or firefox
  engine = ENGINE_SPIDERMONKEY_TRUNK;
} else if (typeof debug === 'function') {
  engine = ENGINE_JAVASCRIPTCORE;
}

// If WebAssembly object doesn't exist, make it an empty function, else runtime
// flags like --wasm-compiler=ion throw
if (typeof WebAssembly === 'undefined') {
  this.WebAssembly = function() {};
}

if (typeof gc === 'undefined') {
  this.gc = function() {};
}
var gcIsQuiet = true;


function fuzzTestingFunctionsCtor(
    fGlobal) { /* eslint-disable-line require-jsdoc */
  function numberOfInstructions() {
    return Math.floor(Random.ludOneTo(10000));
  } /* eslint-disable-line require-jsdoc */
  function numberOfAllocs() {
    return Math.floor(Random.ludOneTo(500));
  } /* eslint-disable-line require-jsdoc */
  function gcSliceSize() {
    return Math.floor(Random.ludOneTo(0x100000000));
  } /* eslint-disable-line require-jsdoc */
  function maybeCommaShrinking() {
    return rnd(5) ? '' : ', \'shrinking\'';
  } /* eslint-disable-line require-jsdoc */

  function maybeShrinkingLastDitch() { /* eslint-disable-line require-jsdoc */
    switch (rnd(3)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return ', \'shrinking\'';
      case 1:
        return ', \'last-ditch\'';
      default:
        return '';
        /* eslint-enable no-multi-spaces */
    }
  }

  function tryCatch(statement) { /* eslint-disable-line require-jsdoc */
		return statement;
    // return `try { ${statement} } catch${rnd(2) ? '(e)' : ''} { }`;
  }

  function setGcparam() { /* eslint-disable-line require-jsdoc */
    // Some switches from the following, added on 2019-05-07:
    // https://hg.mozilla.org/mozilla-central/file/3c70f36ad62c9c714db319/js/src/builtin/TestingFunctions.cpp#l478
    switch (rnd(12)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return _set('sliceTimeBudgetMS', rnd(100));
      case 1:
        return _set(
            'minNurseryBytes', rnd(2) ? 0 : (1 + rnd(30)));  // See bug 1540670
      case 2:
        return _set(
            'maxNurseryBytes',
            rnd(2) ? rnd(30) : (2 ** 32 - 1));  // See bug 1538594
      case 3:
        return _set('mode', rnd(4));
      case 4:
        return _set('dynamicHeapGrowth', rnd(2));
      case 5:
        return _set('dynamicMarkSlice', rnd(2));
      case 6:
        return _set('compactingEnabled', rnd(2));
      case 7:
      case 8:
        return _set('maxEmptyChunkCount', rnd(40));
      case 9:
        return _set(
            'nurseryFreeThresholdForIdleCollectionPercent', (rnd(100) + 1));
      case 10:
        return _set('pretenureThreshold', (rnd(100) + 1));
      default:
        return _set(
            'markStackLimit',
            rnd(2) ? (1 + rnd(30)) :
                     4294967295);  // Artificially trigger delayed marking
                                   /* eslint-enable no-multi-spaces */
    }

    function _set(name, value) { /* eslint-disable-line require-jsdoc */
      // try..catch because gcparam sets may throw, depending on GC state (see
      // bug 973571)
      return tryCatch(`gcparam('${name}', ${value});`);
    }
  }

  // https://mxr.mozilla.org/mozilla-central/source/js/src/builtin/TestingFunctions.cpp
  var sharedTestingFunctions = [];

  // Functions only in the SpiderMonkey shell
  // https://mxr.mozilla.org/mozilla-central/source/js/src/shell/js.cpp
  var shellOnlyTestingFunctions = [
    // ARM simulator settings
    // These throw when not in the ARM simulator.
    {
      w: 1,
      v: function(d, b) {
        return tryCatch(`(voiddisableSingleStepProfiling())`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return tryCatch(`(enableSingleStepProfiling())`);
      }
    },

    // Force garbage collection with function relazification
    {
      w: 10,
      v: function(d, b) {
        return `void relazifyFunctions();`;
      }
    },
    {
      w: 10,
      v: function(d, b) {
        return `void relazifyFunctions('compartment');`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `void relazifyFunctions(${fGlobal(d, b)});`;
      }
    },

    // Test recomputeWrappers - see bug 1492406
    {
      w: 10,
      v: function(d, b) {
        return `recomputeWrappers();`;
      }
    },
    // Also test recomputeWrappers calling newGlobal, see the bug

    // [TestingFunctions.cpp, but debug-only and CRASHY]
    // After N js_malloc memory allocations, fail every following allocation
    {
      w: 1,
      v: function(d, b) {
        return (typeof oomAfterAllocations === 'function' && rnd(1000) === 0) ?
            `oomAfterAllocations(${numberOfAllocs() - 1});` :
            'void 0;';
      }
    },
    // After N js_malloc memory allocations, fail one allocation
    {
      w: 1,
      v: function(d, b) {
        return (typeof oomAtAllocation === 'function' && rnd(100) === 0) ?
            `oomAtAllocation(${numberOfAllocs() - 1});` :
            'void 0;';
      }
    },
    // Reset either of the above
    {
      w: 1,
      v: function(d, b) {
        return (typeof resetOOMFailure === 'function') ?
            `void resetOOMFailure();` :
            'void 0;';
      }
    }
  ];

  var testingFunctions =
      Random.weighted(sharedTestingFunctions.concat(shellOnlyTestingFunctions));

  return {testingFunctions: testingFunctions, enableGCZeal: false};
}

// If the JavaScript engine being tested has heuristics like
//   "recompile any loop that is run more than X times"
// this should be set to the highest such X.
var HOTLOOP = 60;
function loopCount() {
  return rnd(rnd(HOTLOOP * 3));
} /* eslint-disable-line require-jsdoc */
function loopModulo() {
  return (rnd(2) ? rnd(rnd(HOTLOOP * 2)) : rnd(5)) + 2;
} /* eslint-disable-line require-jsdoc */

function simpleSource(st) { /* eslint-disable-line require-jsdoc */
  function hexify(c) {      /* eslint-disable-line require-jsdoc */
    var code = c.charCodeAt(0);
    var hex = code.toString(16);
    while (hex.length < 4) {
      hex = `0${hex}`;
    }
    return `\\u${hex}`;
  }

  if (typeof st === 'string') {
    return (
        '"' +
        st.replace(/\\/g, '\\\\')
            .replace(/"/g, '\\"')
            .replace(/\0/g, '\\0')
            .replace(/\n/g, '\\n')
            .replace(/[^ -~]/g, hexify) +  // not space (32) through tilde (126)
        '"');
  } else {
    return `${st}`;
  }  // hope this is right ;)  should work for numbers.
}

var haveRealUneval = (typeof uneval === 'function');
if (!haveRealUneval) {
  uneval = simpleSource;
}

if (engine === ENGINE_UNKNOWN) {
  printImportant('Targeting an unknown JavaScript engine!');
} else if (engine === ENGINE_SPIDERMONKEY_TRUNK) {
  printImportant('Targeting SpiderMonkey / Gecko (trunk).');
} else if (engine === ENGINE_JAVASCRIPTCORE) {
  printImportant('Targeting JavaScriptCore / WebKit.');
}
var TOTALLY_RANDOM = 1000;

function getListOfMakers(glob) { /* eslint-disable-line require-jsdoc */
  var r = [];
  for (var f in glob) {
    if (f.indexOf('make') === 0 && typeof glob[f] === 'function' &&
        f !== 'makeFinalizeObserver' && f !== 'makeFakePromise') {
      r.push(glob[f]);
    }
  }
  return r;
}


var HOTLOOP = 12;

function loopCount() {
  return rnd(rnd(HOTLOOP * 3));
}

function loopModulo() {
  return (rnd(2) ? rnd(rnd(HOTLOOP * 2)) : rnd(5)) + 2;
}

function cat(toks) { /* eslint-disable-line require-jsdoc */
  if (rnd(1700) === 0) {
    return totallyRandom(2, ['x']);
  }

  var torture = (rnd(1700) === 57);
  if (torture) {
    dumpln('Torture!!!');
  }

  var s = maybeLineBreak();
  for (var i = 0; i < toks.length; ++i) {
    // Catch bugs in the fuzzer.  An easy mistake is
    //   return /*foo*/ + ...
    // instead of
    //   return "/*foo*/" + ...
    // Unary plus in the first one coerces the string that follows to number!
    if (typeof (toks[i]) !== 'string') {
      dumpln(`Strange item in the array passed to cat: typeof toks[${i}] == ${
          typeof (toks[i])}`);
      dumpln(cat.caller);
      dumpln(cat.caller.caller);
    }

    if (!(torture && rnd(12) === 0)) {
      s += toks[i];
    }

    s += maybeLineBreak();

    if (torture) {
      switch (rnd(120)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
          s += maybeSpace() + totallyRandom(2, ['x']) + maybeSpace();
          break;
        case 5:
          s = `(${s})`;  // randomly parenthesize some *prefix* of it.
          break;
        case 6:
          s = '';  // throw away everything before this point
          break;
        case 7:
          return s;  // throw away everything after this point
        case 8:
          s += UNTERMINATED_COMMENT;
          break;
        case 9:
          s += UNTERMINATED_STRING_LITERAL;
          break;
        case 10:
          if (rnd(2)) {
            s += '(';
          }
          s += UNTERMINATED_REGEXP_LITERAL;
          break;
        default:
      }
    }
  }

  return s;
}

function makeRegisterStompFunction(
    d, b, pure) { /* eslint-disable-line require-jsdoc */
  var args = [];
  var nArgs = (rnd(10) ? rnd(20) : rnd(100)) + 1;
  for (var i = 0; i < nArgs; ++i) {
    args.push(`a${i}`);
  }

  var bv = b.concat(args);

  return (
      `(function(${args.join(', ')}) { ` + makeRegisterStompBody(d, bv, pure) +
      `return ${Random.index(bv)}; ` +
      '})');
}

function makeRegisterStompBody(
    d, b, pure) { /* eslint-disable-line require-jsdoc */
  var bv = b.slice(0);
  var lastRVar = 0;
  var s = '';

  function value() { /* eslint-disable-line require-jsdoc */
    return rnd(3) && bv.length ? Random.index(bv) : `${rnd(10)}`;
  }

  function expr() { /* eslint-disable-line require-jsdoc */
    return value() +
        Random.index([' + ', ' - ', ' / ', ' * ', ' % ', ' | ', ' & ', ' ^ ']) +
        value();
  }

  while (rnd(100)) {
    if (bv.length === 0 || rnd(4)) {
      var newVar = `r${lastRVar}`;
      ++lastRVar;
      s += `var ${newVar} = ${expr()}; `;
      bv.push(newVar);
    } else if (rnd(5) === 0 && !pure) {
      s += `print(${Random.index(bv)}); `;
    } else {
      s += `${Random.index(bv)} = ${expr()}; `;
    }
  }

  return s;
}



// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/* exported makeMathyFunAndTest, makeMathyFunRef, mathInitFCM */
/* global errorToString, makeAsmJSFunction, makeMathFunction,
 * makeMixedTypeArray, NUM_MATH_FUNCTIONS, print, Random */
/* global rnd, TOTALLY_RANDOM, totallyRandom, uneval */

var numericVals = [
  '1', 'Math.PI', '42',
  // Special float values
  '0', '-0', '0/0', '1/0', '-1/0',
  // Boundaries of int, signed, unsigned (near +/- 2^31, +/- 2^32)
  '0x07fffffff', '0x080000000', '0x080000001', '-0x07fffffff', '-0x080000000',
  '-0x080000001', '0x0ffffffff', '0x100000000', '0x100000001', '-0x0ffffffff',
  '-0x100000000', '-0x100000001',
  // Boundaries of double
  'Number.MIN_VALUE', '-Number.MIN_VALUE', 'Number.MAX_VALUE',
  '-Number.MAX_VALUE',
  // Boundaries of maximum safe integer
  'Number.MIN_SAFE_INTEGER', '-Number.MIN_SAFE_INTEGER', '-(2**53-2)',
  '-(2**53)', '-(2**53+2)', 'Number.MAX_SAFE_INTEGER',
  '-Number.MAX_SAFE_INTEGER', '(2**53)-2', '(2**53)', '(2**53)+2',
  // See bug 1350097 - 1.79...e308 is the largest (by module) finite number
  '0.000000000000001', '1.7976931348623157e308'
];

var confusableVals = [
  '0',
  '0.1',
  '-0',
  '\'\'',
  '\'0\'',
  '\'\\0\'',
  '[]',
  '[0]',
  '/0/',
  '\'/0/\'',
  '1',
  '({toString:function(){return \'0\';}})',
  '({valueOf:function(){return 0;}})',
  '({valueOf:function(){return \'0\';}})',
  'false',
  'true',
  'undefined',
  'null',
  '(function(){return 0;})',
  'NaN',
  '(new Boolean(false))',
  '(new Boolean(true))',
  '(new String(\'\'))',
  '(new Number(0))',
  '(new Number(-0))',
  'createIsHTMLDDA()'
];

function hashStr(s) { /* eslint-disable-line require-jsdoc */
  var hash = 0;
  var L = s.length;
  for (var i = 0; i < L; i++) {
    var c = s.charCodeAt(i);
    hash = (Math.imul(hash, 31) + c) | 0;
  }
  return hash;
}

function testMathyFunction(f, inputs) { /* eslint-disable-line require-jsdoc */
  var results = [];
  if (f) {
    for (var j = 0; j < inputs.length; ++j) {
      for (var k = 0; k < inputs.length; ++k) {
        try {
          results.push(f(inputs[j], inputs[k]));
        } catch (e) {
          results.push(errorToString(e));
        }
      }
    }
  }
  /* Use uneval to distinguish -0, 0, "0", etc. */
  /* Use hashStr to shorten the output and keep compare_jit files small. */
  print(hashStr(uneval(results)));
}

function mathInitFCM() { /* eslint-disable-line require-jsdoc */
  // FCM cookie, lines with this cookie are used for compare_jit
  var cookie = '/*F' +
      'CM*/';

  // Replace carriage returns (Windows) with line breaks, if present
  print(cookie + hashStr.toString().replace(/\r/g, '\n').replace(/\n/g, ' '));
  print(
      cookie +
      testMathyFunction.toString().replace(/\r/g, '\n').replace(/\n/g, ' '));
}

function makeMathyFunAndTest(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var i = rnd(NUM_MATH_FUNCTIONS);
  var s = '';

  if (rnd(5)) {
    s += `mathy${i} = ${makeMathFunction(6, b, i)}; `;
  }

  if (rnd(5)) {
    var inputsStr;
    switch (rnd(8)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        inputsStr = makeMixedTypeArray(d - 1, b);
        break;
      case 1:
        inputsStr = `[${Random.subset(confusableVals).join(', ')}]`;
        break;
      default:
        inputsStr = `[${Random.subset(numericVals).join(', ')}]`;
        break;
        /* eslint-enable no-multi-spaces */
    }

    s += `testMathyFunction(mathy${i}, ${inputsStr}); `;
  }

  return s;
}

function makeMathyFunRef(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  return `mathy${rnd(NUM_MATH_FUNCTIONS)}`;
}


const NUM_MATH_FUNCTIONS = 6;

var binaryMathOps = [
  ' * ',   ' / ', ' % ', ' + ',  ' - ',  ' ** ', ' << ', ' >> ',
  ' >>> ', ' < ', ' > ', ' <= ', ' >= ', ' == ', ' != ', ' === ',
  ' !== ', ' & ', ' | ', ' ^ ',  ' && ', ' || ', ' , '
];

var leftUnaryMathOps = [' ! ', ' + ', ' - ', ' ~ '];

// unaryMathFunctions and binaryMathFunctions updated on 2017-01-21 and added
// from:
// https://dxr.mozilla.org/mozilla-central/rev/3cedab21a7e65e6a1c4c2294ecfb5502575a46e3/js/src/jsmath.cpp#1330
// Update to the latest revision as needed.
var unaryMathFunctions = [
  'abs', 'acos', 'acosh', 'asin', 'asinh', 'atan', 'atanh', 'cbrt', 'ceil',
  'clz32', 'cos', 'cosh', 'exp', 'expm1',
  // "floor", // avoid breaking rnd.
  'fround', 'log', 'log2', 'log10', 'log1p',
  // "random", // avoid breaking rnd. avoid non-determinism.
  'round', 'sign', 'sin', 'sinh', 'sqrt', 'tan', 'tanh', 'trunc'
];

// n-ary functions will also be tested with varying numbers of parameters by
// makeFunction
var binaryMathFunctions = [
  'atan2',
  'hypot',  // n-ary
  'imul',
  'max',  // n-ary
  'min',  // n-ary
  'pow'
];

function makeMathFunction(d, b, i) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var ivars = ['x', 'y'];
  if (rnd(10) === 0) {
    // Also use variables from the enclosing scope
    ivars = ivars.concat(b);
  }
  return `(function(x, y) { ${directivePrologue()}return ${
      makeMathExpr(d, ivars, i)}; })`;
}

function makeMathExpr(d, b, i) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  // As depth decreases, make it more likely to bottom out
  if (d < rnd(5)) {
    if (rnd(4)) {
      return Random.index(b);
    }
    return Random.index(numericVals);
  }

  if (rnd(500) === 0 && d > 0) {
    return makeExpr(d - 1, b);
  }

  function r() {
    return makeMathExpr(d - 1, b, i);
  } /* eslint-disable-line require-jsdoc */

  // Frequently, coerce both the inputs and outputs to the same "numeric
  // sub-type" (asm.js formalizes this concept, but JITs may have their own
  // variants)
  var commonCoercion = rnd(10);
  function mc(expr) { /* eslint-disable-line require-jsdoc */
    switch (rnd(3) ? commonCoercion : rnd(10)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return `( + ${expr})`;  // f64 (asm.js)
      case 1:
        return `Math.fround(${expr})`;  // f32
      case 2:
        return `(${expr} | 0)`;  // i32 (asm.js)
      case 3:
        return `(${expr} >>> 0)`;  // u32
      default:
        return expr;
        /* eslint-enable no-multi-spaces */
    }
  }

  if (i > 0 && rnd(10) === 0) {
    // Call a *lower-numbered* mathy function. (This avoids infinite recursion.)
    return mc(`mathy${rnd(i)}(${mc(r())}, ${mc(r())})`);
  }

  if (rnd(20) === 0) {
    return mc(`(${mc(r())} ? ${mc(r())} : ${mc(r())})`);
  }

  switch (rnd(4)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return mc(`(${mc(r())}${Random.index(binaryMathOps)}${mc(r())})`);
    case 1:
      return mc(`(${Random.index(leftUnaryMathOps)}${mc(r())})`);
    case 2:
      return mc(`Math.${Random.index(unaryMathFunctions)}(${mc(r())})`);
    default:
      return mc(
          `Math.${Random.index(binaryMathFunctions)}(${mc(r())}, ${mc(r())})`);
      /* eslint-enable no-multi-spaces */
  }
}

function makeScript(d, ignoredB) { /* eslint-disable-line require-jsdoc */
  return directivePrologue() + makeScriptBody(d, ignoredB);
}

function makeScriptBody(d, ignoredB) { /* eslint-disable-line require-jsdoc */
  if (rnd(3) === 0) {
    return makeMathyFunAndTest(d, ['x']);
  }
  return makeStatement(d, ['x']);
}

function makeScriptForEval(d, b) { /* eslint-disable-line require-jsdoc */
  switch (rnd(4)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return makeExpr(d - 1, b);
    case 1:
      return makeStatement(d - 1, b);
    case 2:
      return makeUseRegressionTest(d, b);
    default:
      return makeScript(d - 3, b);
      /* eslint-enable no-multi-spaces */
  }
}


var makeBuilderStatement;
var makeEvilCallback;

(function setUpBuilderStuff() {
  var ARRAY_SIZE = 20;
  var OBJECTS_PER_TYPE = 3;
  var smallPowersOfTwo =
      [1, 2, 4, 8];  // The largest typed array views are 64-bit aka 8-byte
  function bufsize() {
    return rnd(ARRAY_SIZE) * Random.index(smallPowersOfTwo);
  }                           /* eslint-disable-line require-jsdoc */
  function arrayIndex(d, b) { /* eslint-disable-line require-jsdoc */
    switch (rnd(8)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return m('v');
      case 1:
        return makeExpr(d - 1, b);
      case 2:
        return `({valueOf: function() { ${makeStatement(d, b)}return ${
            rnd(ARRAY_SIZE)}; }})`;
      default:
        return `${rnd(ARRAY_SIZE)}`;
        /* eslint-enable no-multi-spaces */
    }
  }

  // Emit a variable name for type-abbreviation t.
  function m(t) { /* eslint-disable-line require-jsdoc */
    if (!t) {
      t = 'aosmevbtihgfp';
    }
    t = t.charAt(rnd(t.length));
    var name = t + rnd(OBJECTS_PER_TYPE);
    switch (rnd(24)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return m('o');
      case 1:
        return `${m('o')}.${name}`;
      case 2:
        return m('g');
      case 3:
        return `${m('g')}.${name}`;
      case 4:
        return `this.${name}`;
      default:
        return name;
        /* eslint-enable no-multi-spaces */
    }
  }

  function val(d, b) { /* eslint-disable-line require-jsdoc */
    if (rnd(10)) {
      return m();
    }
    return makeExpr(d, b);
  }

  // Emit an assignment (or a roughly-equivalent getter)
  function assign(d, b, t, rhs) { /* eslint-disable-line require-jsdoc */
    switch (rnd(18)) {
      // Could have two forms of the getter: one that computes it each time on
      // demand, and one that computes a constant-function closure
      /* eslint-disable no-multi-spaces */
      case 0:
        return (
            'Object.defineProperty(' +
            `${rnd(8) ? 'this' : m('og')}, ` +
            `${simpleSource(m(t))}, ` +
            `{ ${propertyDescriptorPrefix(d - 1, b)} get: function() { ${
                rnd(8) ? '' :
                         makeBuilderStatement(d - 1, b)} return ${rhs}; } }` +
            ');');
      case 1:
        return `${Random.index(varBinder) + m(t)} = ${rhs};`;
      default:
        return `${m(t)} = ${rhs};`;
        /* eslint-enable no-multi-spaces */
    }
  }

  function makeCounterClosure(d, b) { /* eslint-disable-line require-jsdoc */
    // A closure with a counter. Do stuff depending on the counter.
    var v = uniqueVarName();
    var infrequently = infrequentCondition(v, 10);
    return (
        '(function mcc_() { ' +
        `var ${v} = 0; ` +
        'return function() { ' +
        `++${v}; ` +
        (rnd(3) ? `if (${infrequently}) { dumpln('hit!'); ${
                      makeBuilderStatements(d, b)} } ` +
                 `else { dumpln('miss!'); ${makeBuilderStatements(d, b)} } ` :
                  `${m('f')}(${infrequently});`) +
        '};' +
        '})()');
  }

  function fdecl(d, b) { /* eslint-disable-line require-jsdoc */
    var argName = m();
    var bv = b.concat([argName]);
    return `function ${m('f')}(${argName}) ${makeFunctionBody(d, bv)}`;
  }

  function makeBuilderStatements(d, b) { /* eslint-disable-line require-jsdoc */
    var s = '';
    var extras = rnd(4);
    for (var i = 0; i < extras; ++i) {
      s += `try { ${makeBuilderStatement(d - 2, b)} } catch(e${i}) { } `;
    }
    s += makeBuilderStatement(d - 1, b);
    return s;
  }

  var builderFunctionMakers = Random.weighted([
    {
      w: 9,
      v: function(d, b) {
        return `(function() { ${makeBuilderStatements(d, b)} return ${m()}; })`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `(function() { ${makeBuilderStatements(d, b)} throw ${m()}; })`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `(function(j) { ${m('f')}(j); })`;
      }
    },  // a function that just makes one call is begging to be inlined
    // The following pair create and use boolean-using functions.
    {
      w: 4,
      v: function(d, b) {
        return `(function(j) { if (j) { ${
            makeBuilderStatements(
                d, b)} } else { ${makeBuilderStatements(d, b)} } })`;
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `(function() { for (var j=0;j<${loopCount()};++j) { ${
            m('f')}(j%${2 + rnd(4)}==${rnd(2)}); } })`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${Random.index(builtinFunctions)}.bind(${m()})`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return m('f');
      }
    },
    {w: 3, v: makeCounterClosure}, {w: 2, v: makeFunction},
    {w: 1, v: makeRegisterStompFunction}
  ]);
  makeEvilCallback = function(d, b) {
    return (Random.index(builderFunctionMakers))(d - 1, b);
  };

  var handlerTraps = [
    'getOwnPropertyDescriptor', 'defineProperty', 'ownKeys', 'deleteProperty',
    'has', 'get', 'set', 'getPrototypeOf', 'setPrototypeOf', 'isExtensible',
    'preventExtensions', 'apply', 'construct'
  ];

  function forwardingHandler(d, b) { /* eslint-disable-line require-jsdoc */
    return ('({' +
            'getOwnPropertyDescriptor: function(target, name) { Z; var desc = Reflect.getOwnPropertyDescriptor(X); desc.configurable = true; return desc; }, ' +
            'defineProperty: function(target, name, desc) { Z; return Reflect.defineProperty(X, name, desc); }, ' +
            'ownKeys: function(target) { Z; return Reflect.ownKeys(X); }, ' +
            'deleteProperty: function(target, name) { Z; return Reflect.deleteProperty(X, name); }, ' +
            'has: function(target, name) { Z; return name in X; }, ' +
            'get: function(target, name, receiver) { Z; return Reflect.get(X, name, receiver); }, ' +
            'set: function(target, name, val, receiver) { Z; return Reflect.set(X, name, val, receiver); }, ' +
            'getPrototypeOf: function(target) { Z; return Reflect.getPrototypeOf(X); }' +
            'setPrototypeOf: function(target, proto) { Z; return Reflect.setPrototypeOf(X, proto); }' +
            'isExtensible: function(target) { Z; return Reflect.isExtensible(X); }' +
            'preventExtensions: function(target) { Z; return Reflect.preventExtensions(X); }' +
            'apply: function(target, thisArgument, argumentsList) { Z; return Reflect.apply(X, thisArgument, argumentsList); }' +
            'construct: function(target, argumentsList, newTarget) { Z; return Reflect.construct(X, argumentsList, newTarget); }' +
            '})')
        .replace(/X/g, m())
        .replace(/Z/g, function() {
          switch (rnd(20)) {
            /* eslint-disable no-multi-spaces */
            case 0:
              return `return ${m()}`;
            case 1:
              return `throw ${m()}`;
            default:
              return makeBuilderStatement(d - 2, b);
              /* eslint-enable no-multi-spaces */
          }
        });
  }

  function propertyDescriptorPrefix(
      d, b) { /* eslint-disable-line require-jsdoc */
    return `configurable: ${makeBoolean(d, b)}, enumerable: ${
        makeBoolean(d, b)}, `;
  }

  function strToEval(d, b) { /* eslint-disable-line require-jsdoc */
    switch (rnd(5)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return simpleSource(fdecl(d, b));
      case 1:
        return simpleSource(makeBuilderStatement(d, b));
      default:
        return simpleSource(makeScriptForEval(d, b));
        /* eslint-enable no-multi-spaces */
    }
  }

  function evaluateFlags(d, b) { /* eslint-disable-line require-jsdoc */
    // Options are in js.cpp: Evaluate() and ParseCompileOptions()
    return (
        `({ global: ${m('g')}` +
        `, fileName: ${Random.index(['\'evaluate.js\'', 'null'])}` +
        ', lineNumber: 42' +
        `, isRunOnce: ${makeBoolean(d, b)}` +
        `, noScriptRval: ${makeBoolean(d, b)}` +
        `, saveIncrementalBytecode: ${makeBoolean(d, b)}` +
        `, sourceIsLazy: ${makeBoolean(d, b)}` +
        `, catchTermination: ${makeBoolean(d, b)}` +
        ((rnd(5) === 0) ?
             (((rnd(2) === 0) ? `, element: ${m('o')}` : '') +
              ((rnd(2) === 0) ? `, elementAttributeName: ${m('s')}` : '') +
              ((rnd(2) === 0) ? `, sourceMapURL: ${m('s')}` : '')) :
             '') +
        ' })');
  }

  var initializedEverything = false;
  function initializeEverything(d, b) { /* eslint-disable-line require-jsdoc */
    if (initializedEverything) {
      return ';';
    }
    initializedEverything = true;

    var s = '';
    for (var i = 0; i < OBJECTS_PER_TYPE; ++i) {
      s += `a${i} = []; `;
      s += `o${i} = {}; `;
      s += `s${i} = ''; `;
      s += `r${i} = /x/; `;
      s += `g${i} = ${makeGlobal(d, b)}; `;
      s += `f${i} = function(){}; `;
      s += `m${i} = new WeakMap; `;
      s += `e${i} = new Set; `;
      s += `v${i} = null; `;
      s += `b${i} = new ArrayBuffer(64); `;
      s += `t${i} = new Uint8ClampedArray; `;
      // nothing for iterators, handlers
    }
    return s;
  }

  // Emit a method call expression, in one of the following forms:
  //   Array.prototype.push.apply(a1, [x])
  //   Array.prototype.push.call(a1, x)
  //   a1.push(x)
  function method(
      d, b, clazz, obj, meth, arglist) { /* eslint-disable-line require-jsdoc */
    // Sometimes ignore our arguments
    if (rnd(10) === 0) {
      arglist = [];
    }

    // Stuff in extra arguments
    while (rnd(2)) {
      arglist.push(val(d, b));
    }

    // Emit a method call expression
    switch (rnd(4)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        return `${clazz}.prototype.${meth}.apply(${obj}, [${
            arglist.join(', ')}])`;
      case 1:
        return `${clazz}.prototype.${meth}.call(${
                [obj].concat(arglist).join(', ')})`;
      default:
        return `${obj}.${meth}(${arglist.join(', ')})`;
        /* eslint-enable no-multi-spaces */
    }
  }

  function severalargs(f) { /* eslint-disable-line require-jsdoc */
    var arglist = [];
    arglist.push(f());
    while (rnd(2)) {
      arglist.push(f());
    }
    return arglist;
  }

  var builderStatementMakers = Random.weighted([
    // a: Array
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', '[]');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', 'new Array');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', makeIterable(d, b));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m('a')}.length = ${arrayIndex(d, b)};`;
      }
    },
    {
      w: 8,
      v: function(d, b) {
        return assign(d, b, 'v', `${m('at')}.length`);
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `${m('at')}[${arrayIndex(d, b)}] = ${val(d, b)};`;
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `${val(d, b)} = ${m('at')}[${arrayIndex(d, b)}];`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', `${makeFunOnCallChain(d, b)}.arguments`);
      }
    },  // a read-only arguments object
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', 'arguments');
      }
    },  // a read-write arguments object

    // Array indexing
    {
      w: 3,
      v: function(d, b) {
        return `${m('at')}[${arrayIndex(d, b)}];`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `${m('at')}[${arrayIndex(d, b)}] = ${makeExpr(d, b)};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `/*ADP-1*/Object.defineProperty(${m('a')}, ${
            arrayIndex(d, b)}, ${makePropertyDescriptor(d, b)});`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `/*ADP-2*/Object.defineProperty(${m('a')}, ${
            arrayIndex(d, b)}, { ${propertyDescriptorPrefix(d, b)}get: ${
            makeEvilCallback(d, b)}, set: ${makeEvilCallback(d, b)} });`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `/*ADP-3*/Object.defineProperty(${m('a')}, ${
            arrayIndex(d, b)}, { ${propertyDescriptorPrefix(d, b)}writable: ${
            makeBoolean(d, b)}, value: ${val(d, b)} });`;
      }
    },

    // Array mutators
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'push', severalargs(function() {
                           return val(d, b);
                         }))};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'pop', [])};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${
            method(d, b, 'Array', m('a'), 'unshift', severalargs(function() {
                     return val(d, b);
                   }))};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'shift', [])};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'reverse', [])};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'sort', [makeEvilCallback(
                                                            d, b)])};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'splice', [
          arrayIndex(d, b) - arrayIndex(d, b), arrayIndex(d, b)
        ])};`;
      }
    },
    // Array accessors
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 's', method(d, b, 'Array', m('a'), 'join', [m('s')]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'a',
            method(d, b, 'Array', m('a'), 'concat', severalargs(function() {
                     return m('at');
                   })));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'a', method(d, b, 'Array', m('a'), 'slice', [
                        arrayIndex(d, b) - arrayIndex(d, b),
                        arrayIndex(d, b) - arrayIndex(d, b)
                      ]));
      }
    },

    // Array iterators
    {
      w: 5,
      v: function(d, b) {
        return `${method(d, b, 'Array', m('a'), 'forEach', [makeEvilCallback(
                                                               d, b)])};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'a',
            method(d, b, 'Array', m('a'), 'map', [makeEvilCallback(d, b)]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'a',
            method(d, b, 'Array', m('a'), 'filter', [makeEvilCallback(d, b)]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'v',
            method(d, b, 'Array', m('a'), 'some', [makeEvilCallback(d, b)]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'v',
            method(d, b, 'Array', m('a'), 'every', [makeEvilCallback(d, b)]));
      }
    },

    // Array reduction, either with a starting value or with the default of
    // starting with the first two elements.
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'v',
            method(
                d, b, 'Array', m('a'), Random.index(['reduce, reduceRight']),
                [makeEvilCallback(d, b)]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 'v',
            method(
                d, b, 'Array', m('a'), Random.index(['reduce, reduceRight']),
                [makeEvilCallback(d, b), val(d, b)]));
      }
    },

    // Typed Objects (aka Binary Data)
    // http://wiki.ecmascript.org/doku.php?id=harmony:typed_objects (does not
    // match what's in spidermonkey as of 2014-02-11) Do I need to keep track of
    // 'types', 'objects of those types', and 'arrays of objects of those
    // types'? { w: 1,  v: function(d, b) { return assign(d, b, "d",
    // `${m("d")}.flatten()`); } }, { w: 1,  v: function(d, b) { return
    // assign(d, b, "d", `${m("d")}.partition(${rnd(2) ? m("v") : rnd(10)})`); }
    // },

    // o: Object
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'o', '{}');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'o', 'new Object');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'o', `Object.create(${val(d, b)})`);
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `selectforgc(${m('o')});`;
      }
    },

    // s: String
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 's', '\'\'');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 's', 'new String');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 's', `new String(${m()})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 's', `${m('s')}.charAt(${arrayIndex(d, b)})`);
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('s')} += 'x';`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('s')} += ${m('s')};`;
      }
    },
    // Should add substr, substring, replace

    // m: Map, WeakMap
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'm', 'new Map');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'm', `new Map(${m()})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'm', 'new WeakMap');
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('m')}.has(${val(d, b)});`;
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `${m('m')}.get(${val(d, b)});`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, null, `${m('m')}.get(${val(d, b)})`);
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('m')}.set(${val(d, b)}, ${val(d, b)});`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `${m('m')}.delete(${val(d, b)});`;
      }
    },

    // e: Set
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'e', 'new Set');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'e', `new Set(${m()})`);
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('e')}.has(${val(d, b)});`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m('e')}.add(${val(d, b)});`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `${m('e')}.delete(${val(d, b)});`;
      }
    },

    // b: Buffer
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'b', `new ${arrayBufferType()}(${bufsize()})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'b', `${m('t')}.buffer`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `neuter(${m('b')}, ${
            rnd(2) ? '"same-data"' : '"change-data"'});`;
      }
    },

    // t: Typed arrays, aka ArrayBufferViews
    // Can be constructed using a length, typed array, sequence (e.g. array), or
    // buffer with optional offsets!
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 't',
            `new ${Random.index(typedArrayConstructors)}(${arrayIndex(d, b)})`);
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return assign(
            d, b, 't',
            `new ${Random.index(typedArrayConstructors)}(${m('abt')})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 't',
            `new ${Random.index(typedArrayConstructors)}(${m('b')}, ${
                bufsize()}, ${arrayIndex(d, b)})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 't', `${m('t')}.subarray(${arrayIndex(d, b)})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(
            d, b, 't',
            `${m('t')}.subarray(${arrayIndex(d, b)}, ${arrayIndex(d, b)})`);
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `${m('t')}.set(${m('at')}, ${arrayIndex(d, b)});`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `${m('tb')}.byteLength`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `${m('t')}.byteOffset`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `${m('t')}.BYTES_PER_ELEMENT`);
      }
    },

    // h: proxy handler
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'h', '{}');
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'h', forwardingHandler(d, b));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `delete ${m('h')}.${Random.index(handlerTraps)};`;
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `${m('h')}.${Random.index(handlerTraps)} = ${
            makeEvilCallback(d, b)};`;
      }
    },
    {
      w: 4,
      v: function(d, b) {
        return `${m('h')}.${Random.index(handlerTraps)} = ${m('f')};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, null, `new Proxy(${m()}, ${m('h')})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'f', `new Proxy(${m('f')}, ${m('h')})`);
      }
    },

    // g: global or sandbox
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'g', makeGlobal(d, b));
      }
    },
    // Mix builtins between globals
    {
      w: 3,
      v: function(d, b) {
        return `/*MXX1*/${
            assign(d, b, 'o', m('g') + '.' + Random.index(builtinProperties))}`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `/*MXX2*/${m('g')}.${Random.index(builtinProperties)} = ${m()};`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        var prop = Random.index(builtinProperties);
        return `/*MXX3*/${m('g')}.${prop} = ${m('g')}.${prop};`;
      }
    },

    // f: function (?)
    // Could probably do better with args / b
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'f', makeEvilCallback(d, b));
      }
    },
    {w: 1, v: fdecl},
    {
      w: 2,
      v: function(d, b) {
        return `${m('f')}(${m()});`;
      }
    },

    // v: Primitive
    {
      w: 2,
      v: function(d, b) {
        return assign(d, b, 'v', Random.index([
          '4', '4.2', 'NaN', '0', '-0', 'Infinity', '-Infinity'
        ]));
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `new Number(${Random.index([
                        '4', '4.2', 'NaN', '0', '-0', 'Infinity', '-Infinity'
                      ])})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `new Number(${m()})`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', makeBoolean(d, b));
      }
    },
    {
      w: 2,
      v: function(d, b) {
        return assign(
            d, b, 'v', Random.index(['undefined', 'null', 'true', 'false']));
      }
    },

    // evil things we can do to any object property
    {
      w: 1,
      v: function(d, b) {
        return `/*ODP-1*/Object.defineProperty(${m()}, ${
            makePropertyName(d, b)}, ${makePropertyDescriptor(d, b)});`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `/*ODP-2*/Object.defineProperty(${m()}, ${
            makePropertyName(d, b)}, { ${propertyDescriptorPrefix(d, b)}get: ${
            makeEvilCallback(d, b)}, set: ${makeEvilCallback(d, b)} });`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `/*ODP-3*/Object.defineProperty(${m()}, ${
            makePropertyName(
                d, b)}, { ${propertyDescriptorPrefix(d, b)}writable: ${
            makeBoolean(d, b)}, value: ${val(d, b)} });`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `delete ${m()}[${makePropertyName(d, b)}];`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return assign(d, b, 'v', `${m()}[${makePropertyName(d, b)}]`);
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m()}[${makePropertyName(d, b)}] = ${val(d, b)};`;
      }
    },

    // evil things we can do to any object
    {
      w: 5,
      v: function(d, b) {
        return `print(${m()});`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `print(uneval(${m()}));`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m()}.toString = ${makeEvilCallback(d, b)};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m()}.valueOf = ${makeEvilCallback(d, b)};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m()} = ${m()};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m()} = ${m('g')}.createIsHTMLDDA();`;
      }
    },
    {
      w: 3,
      v: function(d, b) {
        return `${m('o')} = ${m()}.__proto__;`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m()}.__proto__ = {};`;
      }
    },
    {
      w: 5,
      v: function(d, b) {
        return `${m()}.__proto__ = ${m()};`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `for (var p in ${m()}) { ${makeBuilderStatements(d, b)} }`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `for (var v of ${m()}) { ${makeBuilderStatements(d, b)} }`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m()} + ${m()};`;
      }
    },  // valueOf
    {
      w: 1,
      v: function(d, b) {
        return `${m()} + '';`;
      }
    },  // toString
    {
      w: 1,
      v: function(d, b) {
        return `${m('v')} = (${m()} instanceof ${m()});`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `${m('v')} = Object.prototype.isPrototypeOf.call(${m()}, ${
            m()});`;
      }
    },
    {
      w: 2,
      v: function(d, b) {
        return `Object.${
            Random.index(['preventExtensions', 'seal', 'freeze'])}(${m()});`;
      }
    },

    // Be promiscuous with the rest of jsfunfuzz
    {
      w: 1,
      v: function(d, b) {
        return `${m()} = x;`;
      }
    },
    {
      w: 1,
      v: function(d, b) {
        return `x = ${m()};`;
      }
    },
    {w: 5, v: makeStatement},

    // Stick in empty for-loops
    {
      w: 5,
      v: function(d, b) {
        return 'for (let i = 0; i < 999; i++) {}';
      }
    },

    {w: 5, v: initializeEverything}
  ]);
  makeBuilderStatement = function(d, b) {
    return (Random.index(builderStatementMakers))(d - 1, b);
  };
})();

function infrequentCondition(v, n) { /* eslint-disable-line require-jsdoc */
  switch (rnd(20)) {
    case 0:
      return true;
    case 1:
      return false;
    case 2:
      return `${v} > ${rnd(n)}`;
    default:
      var mod = rnd(n) + 2;
      var target = rnd(mod);
      return `/*ICCD*/${v} % ${mod}${rnd(8) ? ' == ' : ' != '}${target}`;
  }
}

var arrayBufferType = (typeof SharedArrayBuffer !== 'undefined') ? function() {
  return rnd(2) ? 'SharedArrayBuffer' : 'ArrayBuffer';
} : function() {
  return 'ArrayBuffer';
};

// Statement or block of statements
function makeStatement(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(2)) {
    return makeBuilderStatement(d, b);
  }

  if (d < 6 && rnd(3) === 0) {
    return makePrintStatement(d, b);
  }

  if (d < rnd(8)) {  // frequently for small depth, infrequently for large depth
    return makeLittleStatement(d, b);
  }

  d = rnd(d);  // !

  return (Random.index(statementMakers))(d, b);
}

var varBinder = ['var ', 'let ', 'const ', ''];
var varBinderFor =
    ['var ', 'let ', ''];  // const is a syntax error in for loops

// The reason there are several types of loops here is to create different
// types of scripts without introducing infinite loops.

function forLoopHead(d, b, v, reps) { /* eslint-disable-line require-jsdoc */
  var sInit = '';
  var sCond = '';
  var sNext = '';

  switch (rnd(2)) {
    case 0:  // Generates constructs like `for (var x = 3; x > 0; x--) { ... }`
      sInit = `${Random.index(varBinderFor) + v} = ${reps}`;
      sCond = `${v} > 0`;
      if (rnd(2)) {
        sNext = `--${v}`;
      } else {
        sNext = `${v}--`;
      }
      break;
    default:  // Generates constructs like `for (var x = 0; x < 3; x++) { ... }`
      sInit = `${Random.index(varBinderFor) + v} = 0`;
      sCond = `${v} < ${reps}`;
      if (rnd(2)) {
        sNext = `++${v}`;
      } else {
        sNext = `${v}++`;
      }
  }

  while (rnd(10) === 0) {
    sInit += `, ${makeLetHeadItem(d - 2, b)}`;
  }
  while (rnd(10) === 0) {
    sInit += `, ${makeExpr(d - 2, b)}`;
  }  // NB: only makes sense if our varBinder is ""
  while (rnd(1000) === 0) {
    // never causes the loop to be run, but stuff like register allocation may
    // be happening in the background
    sInit = Random.index(varBinderFor) + v;
  }
  while (rnd(10000) === 0) {
    sInit = '';
  }  // mostly throws ReferenceError, so make this rare

  while (rnd(20) === 0) {
    sCond = `${sCond} && (${makeExpr(d - 2, b)})`;
  }
  while (rnd(20) === 0) {
    sCond = `(${makeExpr(d - 2, b)}) && ${sCond}`;
  }

  while (rnd(20) === 0) {
    sNext = `${sNext}, ${makeExpr(d - 2, b)}`;
  }
  while (rnd(20) === 0) {
    sNext = `${makeExpr(d - 2, b)}, ${sNext}`;
  }

  return `for (${sInit}; ${sCond}; ${sNext})`;
}

function makeOpaqueIdiomaticLoop(d, b) { /* eslint-disable-line require-jsdoc */
  var reps = loopCount();
  var vHidden = uniqueVarName();
  return `/*oLoop*/${forLoopHead(d, b, vHidden, reps)} { ` +
      makeStatement(d - 2, b) + ` } `;
}

function makeTransparentIdiomaticLoop(
    d, b) { /* eslint-disable-line require-jsdoc */
  var reps = loopCount();
  var vHidden = uniqueVarName();
  var vVisible = makeNewId(d, b);
  return `/*vLoop*/${forLoopHead(d, b, vHidden, reps)}` +
      ` { ` +
      `${Random.index(varBinder) + vVisible} = ${vHidden}; ` +
      makeStatement(d - 2, b.concat([vVisible])) + ` } `;
}

function makeBranchUnstableLoop(d, b) { /* eslint-disable-line require-jsdoc */
  var reps = loopCount();
  var v = uniqueVarName();
  var mod = loopModulo();
  var target = rnd(mod);
  return `/*bLoop*/${forLoopHead(d, b, v, reps)} { ` +
      `if (${v} % ${mod} == ${target}) { ${makeStatement(d - 2, b)} } ` +
      `else { ${makeStatement(d - 2, b)} } ` +
      ` } `;
}

function makeTypeUnstableLoop(d, b) { /* eslint-disable-line require-jsdoc */
  var a = makeMixedTypeArray(d, b);
  var v = makeNewId(d, b);
  var bv = b.concat([v]);
  return `/*tLoop*/for (let ${v} of ${a}) { ${makeStatement(d - 2, bv)} }`;
}

function makeFunOnCallChain(d, b) { /* eslint-disable-line require-jsdoc */
  var s = 'arguments.callee';
  while (rnd(2)) {
    s += '.caller';
  }
  return s;
}

var statementMakers = Random.weighted([

  // Any two statements in sequence
  {
    w: 15,
    v: function(d, b) {
      return cat([makeStatement(d - 1, b), makeStatement(d - 1, b)]);
    }
  },
  {
    w: 15,
    v: function(d, b) {
      return cat(
          [makeStatement(d - 1, b), '\n', makeStatement(d - 1, b), '\n']);
    }
  },

  // Stripping semilcolons.  What happens if semicolons are missing?  Especially
  // with line breaks used in place of semicolons (semicolon insertion).
  {
    w: 1,
    v: function(d, b) {
      return cat(
          [stripSemicolon(makeStatement(d, b)), '\n', makeStatement(d, b)]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([stripSemicolon(makeStatement(d, b)), '\n']);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return stripSemicolon(makeStatement(d, b));
    }
  },  // usually invalid, but can be ok e.g. at the end of a block with curly
      // braces

  // Simple variable declarations, followed (or preceded) by statements using
  // those variables
  {
    w: 4,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        Random.index(varBinder), v, ' = ', makeExpr(d, b), ';',
        makeStatement(d - 1, b.concat([v]))
      ]);
    }
  },
  {
    w: 4,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        makeStatement(d - 1, b.concat([v])), Random.index(varBinder), v, ' = ',
        makeExpr(d, b), ';'
      ]);
    }
  },

  // Complex variable declarations, e.g. "const [a,b] = [3,4];" or "var
  // a,b,c,d=4,e;"
  {
    w: 10,
    v: function(d, b) {
      return cat([
        Random.index(varBinder), makeLetHead(d, b), ';', makeStatement(d - 1, b)
      ]);
    }
  },

  // Blocks
  {
    w: 2,
    v: function(d, b) {
      return cat(['{', makeStatement(d, b), ' }']);
    }
  },
  {
    w: 2,
    v: function(d, b) {
      return cat(['{', makeStatement(d - 1, b), makeStatement(d - 1, b), ' }']);
    }
  },

  /* eslint-disable no-multi-spaces */
  // "with" blocks
  {
    w: 2,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'with', '(', makeExpr(d, b), ')',
        makeStatementOrBlock(d, b)
      ]);
    }
  },
  {
    w: 2,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), 'with', '(', '{', v, ': ', makeExpr(d, b), '}', ')',
        makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  /* eslint-enable no-multi-spaces */

  /* eslint-disable no-multi-spaces */
  // C-style "for" loops
  // Two kinds of "for" loops: one with an expression as the first part, one
  // with a var or let binding 'statement' as the first part. I'm not sure if
  // arbitrary statements are allowed there; I think not.
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', makeExpr(d, b), '; ', makeExpr(d, b), '; ',
        makeExpr(d, b), ') ', makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), v, '; ',
        makeExpr(d, b), '; ', makeExpr(d, b), ') ',
        makeStatementOrBlock(d, b.concat([v]))
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), v, ' = ',
        makeExpr(d, b), '; ', makeExpr(d, b), '; ', makeExpr(d, b), ') ',
        makeStatementOrBlock(d, b.concat([v]))
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor),
        makeDestructuringLValue(d, b), ' = ', makeExpr(d, b), '; ',
        makeExpr(d, b), '; ', makeExpr(d, b), ') ', makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  /* eslint-enable no-multi-spaces */

  // Various types of "for" loops, specially set up to test tracing, carefully
  // avoiding infinite loops
  {w: 6, v: makeTransparentIdiomaticLoop},
  {w: 6, v: makeOpaqueIdiomaticLoop},
  {w: 6, v: makeBranchUnstableLoop},
  {w: 8, v: makeTypeUnstableLoop},

  /* eslint-disable no-multi-spaces */
  // "for..in" loops
  // arbitrary-LHS marked as infloop because
  // -- for (key in obj)
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor),
        makeForInLHS(d, b), ' in ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), v, ' in ',
        makeExpr(d - 2, b), ') ', makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  // -- for (key in generator())
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor),
        makeForInLHS(d, b), ' in ', '(', '(', makeFunction(d, b), ')', '(',
        makeExpr(d, b), ')', ')', ')', makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), v, ' in ', '(',
        '(', makeFunction(d, b), ')', '(', makeExpr(d, b), ')', ')', ')',
        makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  // -- for (element of arraylike)
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), ' for ', '(', Random.index(varBinderFor),
        makeLValue(d, b), ' of ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), ' for ', '(', Random.index(varBinderFor), v, ' of ',
        makeExpr(d - 2, b), ') ', makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  /* eslint-enable no-multi-spaces */

  /* eslint-disable no-multi-spaces */
  // -- for-await-of
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), ' for ', 'await', '(', Random.index(varBinderFor),
        makeLValue(d, b), ' of ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), ' for ', 'await', '(', Random.index(varBinderFor), v,
        ' of ', makeExpr(d - 2, b), ') ', makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  /* eslint-enable no-multi-spaces */

  // Modify something during a loop -- perhaps the thing being looped over
  // Since we use "let" to bind the for-variables, and only do wacky stuff once,
  // I *think* this is unlikely to hang.
  //  function(d, b) { return `let forCount = 0; for (let ${makeId(d, b)} in
  //  ${makeExpr(d, b)}) { if (forCount++ == ${rnd(3)}) { ${makeStatement(d - 1,
  //  b)} } }`; },

  /* eslint-disable no-multi-spaces */
  // Hoisty "for..in" loops.  I don't know why this construct exists, but it
  // does, and it hoists the initial-value expression above the loop. With "var"
  // or "const", the entire thing is hoisted. With "let", only the value is
  // hoisted, and it can be elim'ed as a useless statement. The last form is
  // specific to JavaScript 1.7 (only).
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), makeId(d, b),
        ' = ', makeExpr(d, b), ' in ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b)
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      return cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), v, ' = ',
        makeExpr(d, b), ' in ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b.concat([v]))
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      var v = makeNewId(d, b);
      var w = makeNewId(d, b);
      return cat([
        maybeLabel(), 'for', '(', Random.index(varBinderFor), '[', v, ', ', w,
        ']', ' = ', makeExpr(d, b), ' in ', makeExpr(d - 2, b), ') ',
        makeStatementOrBlock(d, b.concat([v, w]))
      ]);
    }
  },
  /* eslint-enable no-multi-spaces */

  // do..while
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'while((', makeExpr(d, b),
        ') && 0)' /* don't split this, it's needed to avoid marking as infloop
                   */
        ,
        makeStatementOrBlock(d, b)
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'while', '(', makeExpr(d, b), ')',
        makeStatementOrBlock(d, b)
      ])}`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'do ', makeStatementOrBlock(d, b), ' while((',
        makeExpr(d, b), ') && 0)' /* don't split this, it's needed to avoid
                                     marking as infloop */
        ,
        ';'
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `/*infloop*/${cat([
        maybeLabel(), 'do ', makeStatementOrBlock(d, b), ' while', '(',
        makeExpr(d, b), ');'
      ])}`;
    }
  },

  // Switch statement
  {
    w: 3,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'switch', '(', makeExpr(d, b), ')', ' { ',
        makeSwitchBody(d, b), ' }'
      ]);
    }
  },

  // Conditionals, perhaps with 'else if' / 'else'
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ', makeStatementOrBlock(d, b)
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ',
        makeStatementOrBlock(d - 1, b), ' else ', makeStatementOrBlock(d - 1, b)
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ',
        makeStatementOrBlock(d - 1, b), ' else ', ' if ', '(', makeExpr(d, b),
        ') ', makeStatementOrBlock(d - 1, b)
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ',
        makeStatementOrBlock(d - 1, b), ' else ', ' if ', '(', makeExpr(d, b),
        ') ', makeStatementOrBlock(d - 1, b), ' else ',
        makeStatementOrBlock(d - 1, b)
      ]);
    }
  },

  // A tricky pair of if/else cases.
  // In the SECOND case, braces must be preserved to keep the final "else"
  // associated with the first "if".
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ', '{', ' if ', '(',
        makeExpr(d, b), ') ', makeStatementOrBlock(d - 1, b), ' else ',
        makeStatementOrBlock(d - 1, b), '}'
      ]);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return cat([
        maybeLabel(), 'if(', makeBoolean(d, b), ') ', '{', ' if ', '(',
        makeExpr(d, b), ') ', makeStatementOrBlock(d - 1, b), '}', ' else ',
        makeStatementOrBlock(d - 1, b)
      ]);
    }
  },

  // Expression statements
  {
    w: 5,
    v: function(d, b) {
      return cat([makeExpr(d, b), ';']);
    }
  },
  {
    w: 5,
    v: function(d, b) {
      return cat(['(', makeExpr(d, b), ')', ';']);
    }
  },

  // Exception-related statements :)
  {
    w: 6,
    v: function(d, b) {
      return makeExceptionyStatement(d - 1, b) +
          makeExceptionyStatement(d - 1, b);
    }
  },
  {
    w: 7,
    v: function(d, b) {
      return makeExceptionyStatement(d, b);
    }
  },

  // Labels. (JavaScript does not have goto, but it does have break-to-label and
  // continue-to-label).
  {
    w: 1,
    v: function(d, b) {
      return cat(['L', ': ', makeStatementOrBlock(d, b)]);
    }
  },

  // Function-declaration-statements with shared names
  {
    w: 10,
    v: function(d, b) {
      return cat([
        makeStatement(d - 2, b), 'function ', makeId(d, b), '(',
        makeFormalArgList(d, b), ')', makeFunctionBody(d - 1, b),
        makeStatement(d - 2, b)
      ]);
    }
  },

  // Function-declaration-statements with unique names, along with calls to
  // those functions
  {w: 8, v: makeNamedFunctionAndUse},

  // Long script -- can confuse Spidermonkey's short vs long jmp or something
  // like that. Spidermonkey's regexp engine is so slow for long strings that we
  // have to bypass whatToTest :( { w: 1, v: function(d, b) { return
  // strTimes("try{}catch(e){}", rnd(10000)); } },
  {
    w: 1,
    v: function(d, b) {
      if (rnd(200) === 0)
        return `/*DUPTRY${rnd(10000)}*/${makeStatement(d - 1, b)}`;
      return ';';
    }
  },

  {
    w: 1,
    v: function(d, b) {
      return makeShapeyConstructorLoop(d, b);
    }
  },

  // Replace a variable with a long linked list pointing to it.  (Forces
  // SpiderMonkey's GC marker into a stackless mode.)
  {
    w: 1,
    v: function(d, b) {
      var x = makeId(d, b);
      return `${x} = ${linkedList(x, (rnd(100) * rnd(100)))}`;
    }
  },

  // Oddly placed "use strict" or "use asm"
  {
    w: 1,
    v: function(d, b) {
      return directivePrologue() + makeStatement(d - 1, b);
    }
  },

  // Spidermonkey GC and JIT controls
  {
    w: 3,
    v: function(d, b) {
      return makeTestingFunctionCall(d, b);
    }
  },
  {
    w: 3,
    v: function(d, b) {
      return `${makeTestingFunctionCall(d - 1, b)} ${makeStatement(d - 1, b)}`;
    }
  },

  // Blocks of statements related to typed arrays
  {w: 8, v: makeTypedArrayStatements},

  // Print statements
  {w: 8, v: makePrintStatement},

  // {w: 20, v: makeRegexUseBlock},

  {w: 1, v: makeRegisterStompBody},

  {w: 20, v: makeUseRegressionTest}

  // Discover properties to add to the allPropertyNames list
  // { w: 3, v: function(d, b) { return `for (var p in ${makeId(d, b)}) {
  // addPropertyName(p); }`; } }, { w: 3, v: function(d, b) { return `var opn =
  // Object.getOwnPropertyNames(${makeId(d, b)}); for (var j = 0; j <
  // opn.length; ++j) { addPropertyName(opn[j]); }`; } },
]);

if (typeof oomTest === 'function' && engine !== ENGINE_JAVASCRIPTCORE) {
  statementMakers = statementMakers.concat([
    function(d, b) {
      return `oomTest(${makeFunction(d - 1, b)})`;
    },
    function(d, b) {
      return `oomTest(${makeFunction(d - 1, b)}, { keepFailing: true })`;
    }
  ]);
}

function makeUseRegressionTest(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (typeof regressionTestList !== 'object') {
    return '/* no regression tests found */';
  }

  var maintest = regressionTestsRoot + Random.index(regressionTestList);
  var files = regressionTestDependencies(maintest);

  var s = '';

  if (rnd(5) === 0) {
    // Many tests call assertEq, intending to throw if something unexpected
    // happens. Sometimes, override it with a function that compares but does
    // not throw.
    s += 'assertEq = function(x, y) { if (x != y) { print(0); } }; ';
  }

  for (var i = 0; i < files.length; ++i) {
    var file = files[i];

    if (regressionTestIsEvil(read(file))) {
      continue;
    }

    let testType = 'regression';

    switch (rnd(2)) {
      case 0:
        // simply inline the script -- this is the only one that will work in
        // newGlobal()
        s += `/* ${testType}-test-inline */ ${inlineTest(file)}`;
        break;
      default:
        // run it using load()
        s += `/* ${testType}-test-load */ load(${simpleSource(file)});`;
        break;
        // NB: these scripts will also be run through eval(), evalcx(),
        // evaluate(), evalInWorker()
        //     thanks to other parts of the fuzzer using makeScriptForEval or
        //     makeStatement
    }
  }
  return s;
}

function regressionTestIsEvil(
    contents) { /* eslint-disable-line require-jsdoc */
  if (contents.indexOf('SIMD') !== -1) {
    // Disable SIMD testing until it's more stable (and we can get better
    // stacks?)
    return true;
  }
  if (contents.indexOf('print = ') !== -1) {
    // A testcase that clobbers the |print| function would confuse
    // js_interesting
    return true;
  }
  return false;
}

function inlineTest(filename) { /* eslint-disable-line require-jsdoc */
  // Inline a regression test, adding NODIFF (to disable differential testing)
  // if it calls a testing function that might throw.

  const s = `/* ${filename} */ ${read(filename)}\n`;

  const noDiffTestingFunctions = [
    // These can throw
    'gcparam', 'startgc', 'setJitCompilerOption', 'disableSingleStepProfiling',
    'enableSingleStepProfiling',
    // These return values depending on command-line options, and some
    // regression tests check them
    'isSimdAvailable',  // in 32-bit x86 builds, it depends on whether --no-fpu
                        // is passed in, because --no-fpu also disables SSE
    'hasChild', 'PerfMeasurement'
  ];

  for (var f of noDiffTestingFunctions) {
    if (s.indexOf(f) !== -1) {
      return `/*NODIFF*/ ${s}`;
    }
  }

  return s;
}

function regressionTestDependencies(
    maintest) { /* eslint-disable-line require-jsdoc */
  var files = [];

  if (rnd(3)) {
    // Include the chain of 'shell.js' files in their containing directories
    // (starting from regressionTestsRoot)
    for (var i = regressionTestsRoot.length; i < maintest.length; ++i) {
      if (maintest.charAt(i) === '/' || maintest.charAt(i) === '\\') {
        var shelljs = `${maintest.substr(0, i + 1)}shell.js`;
        if (regressionTestList.indexOf(shelljs) !== -1) {
          files.push(shelljs);
        }
      }
    }

    // Include prologue.js for jit-tests
    if (maintest.indexOf('jit-test') !== -1) {
      files.push(`${libdir}prologue.js`);
    }

    // Include whitelisted shell.js for some tests, e.g. non262/test262 ones
    if (maintest.indexOf('non262') !== -1) {
      files.push(`${
          js_src_tests_dir}shell.js`); /* eslint-disable-line
                                                                                                                                                                                                                                                                                                                                                                  camelcase,no-undef
                                                                                                                                                                                                                                                                                                                                          */
      files.push(`${
          non262_tests_dir}shell.js`); /* eslint-disable-line
                                                                                                                                                                                                                                                                                                                                                                  camelcase,no-undef
                                                                                                                                                                                                                                                                                                                                          */
    }

    if (maintest.indexOf('test262') !== -1) {
      files.push(`${
          js_src_tests_dir}shell.js`); /* eslint-disable-line
                                                                                                                                                                                                                                                                                                                                                                  camelcase,no-undef
                                                                                                                                                                                                                                                                                                                                          */
      files.push(`${
          test262_tests_dir}shell.js`); /* eslint-disable-line
                                                                                                                                                                                                                                                                                                                                                                   camelcase,no-undef
                                                                                                                                                                                                                                                                                                                                           */
    }

    // Include web-platform-test-shims.js and testharness.js for streams tests
    if (maintest.indexOf('web-platform') !== -1) {
      files.push(`${
          js_src_tests_dir}web-platform-test-shims.js`); /* eslint-disable-line
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    camelcase */
      files.push(`${
          w_pltfrm_res_dir}testharness.js`); /* eslint-disable-line camelcase */
    }
  }

  files.push(maintest);
  return files;
}

function linkedList(x, n) { /* eslint-disable-line require-jsdoc */
  for (var i = 0; i < n; ++i) {
    x = {a: x};
  }
  return x;
}

function makeNamedFunctionAndUse(d, b) { /* eslint-disable-line require-jsdoc */
  // Use a unique function name to make it less likely that we'll accidentally
  // make a recursive call
  var funcName = uniqueVarName();
  var formalArgList = makeFormalArgList(d, b);
  var bv = formalArgList.length === 1 ? b.concat(formalArgList) : b;
  var declStatement = cat([
    '/*hhh*/function ', funcName, '(', formalArgList, ')', '{',
    makeStatement(d - 1, bv), '}'
  ]);
  var useStatement;
  if (rnd(2)) {
    // Direct call
    useStatement = cat([funcName, '(', makeActualArgList(d, b), ')', ';']);
  } else {
    // Any statement, allowed to use the name of the function
    useStatement = `/*iii*/${makeStatement(d - 1, b.concat([funcName]))}`;
  }
  if (rnd(2)) {
    return declStatement + useStatement;
  } else {
    return useStatement + declStatement;
  }
}

function makePrintStatement(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(2) && b.length) {
    return `print(${Random.index(b)});`;
  } else {
    return `print(${makeExpr(d, b)});`;
  }
}

function maybeLabel() { /* eslint-disable-line require-jsdoc */
  if (rnd(4) === 1) {
    return cat([Random.index(['L', 'M']), ':']);
  } else {
    return '';
  }
}

function uniqueVarName() { /* eslint-disable-line require-jsdoc */
  // Make a random variable name.
  var s = '';
  for (var i = 0; i < 6; ++i) {
    s += String.fromCharCode(97 + rnd(26));
  }  // a lowercase english letter
  return s;
}

function makeSwitchBody(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var haveSomething = false;
  var haveDefault = false;
  var output = '';

  do {
    if (!haveSomething || rnd(2)) {
      // Want a case/default (or, if this is the beginning, "need").

      if (!haveDefault && rnd(2)) {
        output += 'default: ';
        haveDefault = true;
      } else {
        // cases with numbers (integers?) have special optimizations,
        // so be sure to test those well in addition to testing complicated
        // expressions.
        output += `case ${rnd(2) ? rnd(10) : makeExpr(d, b)}: `;
      }

      haveSomething = true;
    }

    // Might want a statement.
    if (rnd(2)) {
      output += makeStatement(d, b);
    }

    // Might want to break, or might want to fall through.
    if (rnd(2)) {
      output += 'break; ';
    }

    if (rnd(2)) {
      --d;
    }
  } while (d && rnd(5));

  return output;
}

function makeLittleStatement(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  d = d - 1;

  if (rnd(4) === 1) {
    return makeStatement(d, b);
  }

  return (Random.index(littleStatementMakers))(d, b);
}

var littleStatementMakers = [
  // Tiny
  function(d, b) {
    return cat([';']);
  },  // e.g. empty "if" block
  function(d, b) {
    return cat(['{', '}']);
  },  // e.g. empty "if" block
  function(d, b) {
    return cat(['']);
  },

  // Throw stuff.
  function(d, b) {
    return cat(['throw ', makeExpr(d, b), ';']);
  },

  // Break/continue [to label].
  function(d, b) {
    return cat([
      Random.index(['continue', 'break']), ' ',
      Random.index(['L', 'M', '', '']), ';'
    ]);
  },

  // Named and unnamed functions (which have different behaviors in different
  // places: both can be expressions, but unnamed functions "want" to be
  // expressions and named functions "want" to be special statements)
  function(d, b) {
    return makeFunction(d, b);
  },

  // Return, yield, await
  function(d, b) {
    return cat(['return ', makeExpr(d, b), ';']);
  },
  function(d, b) {
    return 'return;';
  },  // return without a value is allowed in generators; return with a value is
      // not.
  function(d, b) {
    return cat(['yield ', makeExpr(d, b), ';']);
  },  // note: yield can also be a left-unary operator, or something like that
  function(d, b) {
    return 'yield;';
  },
  function(d, b) {
    return cat(['await ', makeExpr(d, b), ';']);
  },

  // Expression statements
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', ';']);
  }
];

// makeStatementOrBlock exists because often, things have different behaviors
// depending on where there are braces.
function makeStatementOrBlock(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  return (Random.index(statementBlockMakers))(d - 1, b);
}

var statementBlockMakers = [
  function(d, b) {
    return makeStatement(d, b);
  },
  function(d, b) {
    return makeStatement(d, b);
  },
  function(d, b) {
    return cat(['{', makeStatement(d, b), ' }']);
  },
  function(d, b) {
    return cat(['{', makeStatement(d - 1, b), makeStatement(d - 1, b), ' }']);
  }
];

// Extra-hard testing for try/catch/finally and related things.

function makeExceptionyStatement(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  d = d - 1;
  if (d < 1) {
    return makeLittleStatement(d, b);
  }

  return (Random.index(exceptionyStatementMakers))(d, b);
}

var exceptionProperties =
    ['constructor', 'message', 'name', 'fileName', 'lineNumber', 'stack'];

var exceptionyStatementMakers = [
  function(d, b) {
    return makeTryBlock(d, b);
  },

  function(d, b) {
    return makeStatement(d, b);
  },
  function(d, b) {
    return makeLittleStatement(d, b);
  },

  function(d, b) {
    return 'return;';
  },  // return without a value can be mixed with yield
  function(d, b) {
    return cat(['return ', makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat(['yield ', makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat(['await ', makeExpr(d, b), ';']);
  },
  function(d, b) {
    return cat(['throw ', makeId(d, b), ';']);
  },
  function(d, b) {
    return `${b[b.length - 1]}.${Random.index(exceptionProperties)};`;
  },
  function(d, b) {
    return `${makeId(d, b)}.${Random.index(exceptionProperties)};`;
  },
  function(d, b) {
    return cat([makeId(d, b), ' = ', makeId(d, b), ';']);
  },
  function(d, b) {
    return cat([makeLValue(d, b), ' = ', makeId(d, b), ';']);
  },

  // Iteration is also useful to test because it asserts that there is no
  // pending exception.
  function(d, b) {
    var v = makeNewId(d, b);
    return `for(let ${v} in []);`;
  },
  function(d, b) {
    var v = makeNewId(d, b);
    return `for(let ${v} in ${makeIterable(d, b)}) ${
        makeExceptionyStatement(d, b.concat([v]))}`;
  },
  function(d, b) {
    var v = makeNewId(d, b);
    return `for(let ${v} of ${makeIterable(d, b)}) ${
        makeExceptionyStatement(d, b.concat([v]))}`;
  },
  function(d, b) {
    var v = makeNewId(d, b);
    return `for await(let ${v} of ${makeIterable(d, b)}) ${
        makeExceptionyStatement(d, b.concat([v]))}`;
  },

  // Scary place to throw: with
  function(d, b) {
    return `with({}) ${makeExceptionyStatement(d, b)}`;
  },
  function(d, b) {
    return `with({}) { ${makeExceptionyStatement(d, b)} } `;
  }
];

function makeTryBlock(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  // The following comment was added *before* guarded/conditional catch support
  // was removed from jsfunfuzz Catches: 1/6 chance of having none Catches:
  // maybe 2 + 1/2 So approximately 4 recursions into makeExceptionyStatement on
  // average! Therefore we want to keep the chance of recursing too much down.

  d = d - rnd(3);

  var s = cat(['try', ' { ', makeExceptionyStatement(d, b), ' } ']);

  var numCatches = 0;

  if (rnd(2)) {
    // Add an unguarded catch.
    ++numCatches;
    let catchId = makeId(d, b);
    let catchBlock = makeExceptionyStatement(d, b.concat([catchId]));
    if (rnd(2)) {
      s += cat(['catch', '(', catchId, ')', ' { ', catchBlock, ' } ']);
    } else {
      s += cat([
        'catch', ' { ', catchBlock, ' } '
      ]);  // Catch bindings are now optional thanks to bug 1380881
    }
  }

  if (numCatches === 0 || rnd(2) === 1) {
    // Add a finally.
    s += cat(['finally', ' { ', makeExceptionyStatement(d, b), ' } ']);
  }

  return s;
}

// Creates a string that sorta makes sense as an expression
function makeExpr(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (d <= 0 || (rnd(7) === 1)) {
    return makeTerm(d - 1, b);
  }

  if (rnd(6) === 1 && b.length) {
    return Random.index(b);
  }

  /* RECURSION
  if (rnd(10) === 1) {
    return makeImmediateRecursiveCall(d, b);
  }
        */

  d = rnd(d);  // !

  var expr = (Random.index(exprMakers))(d, b);

  if (rnd(4) === 1) {
    return `(${expr})`;
  } else {
    return expr;
  }
}

var binaryOps = [
  // Long-standing JavaScript operators, roughly in order from
  // http://www.codehouse.com/javascript/precedence/
  ' * ',   ' / ',   ' % ',  ' + ',  ' - ',          ' << ',  ' >> ',  ' >>> ',
  ' < ',   ' > ',   ' <= ', ' >= ', ' instanceof ', ' in ',  ' == ',  ' != ',
  ' === ', ' !== ', ' & ',  ' | ',  ' ^ ',          ' && ',  ' || ',  ' = ',
  ' *= ',  ' /= ',  ' %= ', ' += ', ' -= ',         ' <<= ', ' >>= ', ' >>>= ',
  ' &= ',  ' ^= ',  ' |= ', ' , ',  ' ** ',         ' **= '
];

var leftUnaryOps = [
  '!', '+', '-', '~', 'void ', 'typeof ', 'delete ',
  'new ',    // but note that "new" can also be a very strange left-binary
             // operator
  'yield ',  // see http://www.python.org/dev/peps/pep-0342/ .  Often needs to
             // be parenthesized, so there's also a special exprMaker for it.
  'await '
];

var incDecOps = ['++', '--'];

var specialProperties = [
  '__proto__', 'constructor', 'prototype', 'wrappedJSObject', 'arguments',
  'caller', 'callee', 'toString', 'valueOf', 'call',
  'apply',  // ({apply:...}).apply() hits a special case (speculation failure
            // with funapply / funcall bytecode)
  'length', '0', '1', 'Symbol.species'
];

// This makes it easier for fuzz-generated code to mess with the fuzzer. Will I
// regret it?
/*
function addPropertyName(p)
{
  p = `${p}`;
  if (
      p !== "floor" &&
      p !== "random" &&
      p !== "parent" && // unsafe spidermonkey shell function, see bug 619064
      true) {
    print(`Adding: ${p}`);
    allPropertyNames.push(p);
  }
}
*/

var exprMakers = [
  // Increment and decrement
  function(d, b) {
    return cat([makeLValue(d, b), Random.index(incDecOps)]);
  },
  function(d, b) {
    return cat([Random.index(incDecOps), makeLValue(d, b)]);
  },

  // Other left-unary operators
  function(d, b) {
    return cat([Random.index(leftUnaryOps), makeExpr(d, b)]);
  },

  // Methods
  function(d, b) {
    var id = makeId(d, b);
    return cat([
      '/*UUV1*/', '(', id, '.', Random.index(allMethodNames), ' = ',
      makeFunction(d, b), ')'
    ]);
  },
  function(d, b) {
    var id = makeId(d, b);
    return cat([
      '/*UUV2*/', '(', id, '.', Random.index(allMethodNames), ' = ', id, '.',
      Random.index(allMethodNames), ')'
    ]);
  },
  function(d, b) {
    return cat([
      makeExpr(d, b), '.', Random.index(allMethodNames), '(',
      makeActualArgList(d, b), ')'
    ]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), '.', 'valueOf', '(', uneval('number'), ')']);
  },

  // Binary operators
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), Random.index(binaryOps), makeExpr(d, b)]);
  },
  function(d, b) {
    let expr = makeExpr(d, b);
    return cat(['/*infloop*/', expr, Random.index(binaryOps), expr]);
  },
  function(d, b) {
    return cat([makeId(d, b), Random.index(binaryOps), makeId(d, b)]);
  },
  function(d, b) {
    return cat([makeId(d, b), Random.index(binaryOps), makeId(d, b)]);
  },
  function(d, b) {
    return cat([makeId(d, b), Random.index(binaryOps), makeId(d, b)]);
  },
  function(d, b) {
    let id = makeId(d, b);
    return cat(['/*infloop*/', id, Random.index(binaryOps), id]);
  },

  // Ternary operator
  function(d, b) {
    return cat([makeExpr(d, b), ' ? ', makeExpr(d, b), ' : ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), ' ? ', makeExpr(d, b), ' : ', makeExpr(d, b)]);
  },

  // In most contexts, yield expressions must be parenthesized, so including
  // explicitly parenthesized yields makes actually-compiling yields appear more
  // often.
  function(d, b) {
    return cat(['yield ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat(['(', 'yield ', makeExpr(d, b), ')']);
  },

  function(d, b) {
    return cat(['await ', makeExpr(d, b)]);
  },

  // Test mark bits of objects
  // addMarkObservers may need a better input than makeArrayLiteral, to focus
  // more on valid objects
  function(d, b) {
    return cat(['addMarkObservers', '(', makeArrayLiteral(d, b), ')']);
  },
  function(d, b) {
    return cat(['clearMarkObservers', '()']);
  },
  function(d, b) {
    return cat(['getMarks', '()']);
  },

  // Print the scope chain of objects
  function(d, b) {
    return cat(['dumpScopeChain', '(', makeFunction(d, b), ')']);
  },

  // Array functions (including extras).  The most interesting are map and
  // filter, I think. These are mostly interesting to fuzzers in the sense of
  // "what happens if i do strange things from a filter function?"  e.g. modify
  // the array.. :) This fuzzer isn't the best for attacking this kind of thing,
  // since it's unlikely that the code in the function will attempt to modify
  // the array or make it go away. The second parameter to "map" is used as the
  // "this" for the function.
  function(d, b) {
    return cat([
      makeArrayLiteral(d, b), '.',
      Random.index(['map', 'filter', 'some', 'sort'])
    ]);
  },
  function(d, b) {
    return cat([
      makeArrayLiteral(d, b), '.',
      Random.index(['map', 'filter', 'some', 'sort']), '(', makeFunction(d, b),
      ', ', makeExpr(d, b), ')'
    ]);
  },
  function(d, b) {
    return cat([
      makeArrayLiteral(d, b), '.',
      Random.index(['map', 'filter', 'some', 'sort']), '(', makeFunction(d, b),
      ')'
    ]);
  },

  // RegExp replace.  This is interesting for the same reason as array extras.
  function(d, b) {
    return cat([
      '\'fafafa\'', '.', 'replace', '(', '/', 'a', '/', 'g', ', ',
      makeFunction(d, b), ')'
    ]);
  },

  // Containment in an array or object (or, if this happens to end up on the LHS
  // of an assignment, destructuring)
  function(d, b) {
    return cat(['[', makeExpr(d, b), ']']);
  },
  function(d, b) {
    return cat(['(', '{', makeId(d, b), ': ', makeExpr(d, b), '}', ')']);
  },

  // Functions: called immediately/not
  function(d, b) {
    return makeFunction(d, b);
  },
  function(d, b) {
    return `${makeFunction(d, b)}.prototype`;
  },
  function(d, b) {
    return cat(
        ['(', makeFunction(d, b), ')', '(', makeActualArgList(d, b), ')']);
  },

  // Try to call things that may or may not be functions.
  function(d, b) {
    return cat([makeExpr(d, b), '(', makeActualArgList(d, b), ')']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', '(', makeActualArgList(d, b), ')']);
  },
  function(d, b) {
    return cat([makeFunction(d, b), '(', makeActualArgList(d, b), ')']);
  },

  // Try to test function.call heavily.
  function(d, b) {
    return cat([
      '(', makeFunction(d, b), ')', '.', 'call', '(', makeExpr(d, b), ', ',
      makeActualArgList(d, b), ')'
    ]);
  },

  // Binary "new", with and without clarifying parentheses, with expressions or
  // functions
  function(d, b) {
    return cat(['new ', makeExpr(d, b), '(', makeActualArgList(d, b), ')']);
  },
  function(d, b) {
    return cat(
        ['new ', '(', makeExpr(d, b), ')', '(', makeActualArgList(d, b), ')']);
  },

  function(d, b) {
    return cat(['new ', makeFunction(d, b), '(', makeActualArgList(d, b), ')']);
  },
  function(d, b) {
    return cat([
      'new ', '(', makeFunction(d, b), ')', '(', makeActualArgList(d, b), ')'
    ]);
  },

  // Sometimes we do crazy stuff, like putting a statement where an expression
  // should go.  This frequently causes a syntax error.
  function(d, b) {
    return stripSemicolon(makeLittleStatement(d, b));
  },
  function(d, b) {
    return '';
  },

  // Comments and whitespace
  function(d, b) {
    return cat([' /* Comment */', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat(['\n', makeExpr(d, b)]);
  },  // perhaps trigger semicolon insertion and stuff
  function(d, b) {
    return cat([makeExpr(d, b), '\n']);
  },

  // LValue as an expression
  function(d, b) {
    return cat([makeLValue(d, b)]);
  },

  // Assignment (can be destructuring)
  function(d, b) {
    return cat([makeLValue(d, b), ' = ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeLValue(d, b), ' = ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat(['(', makeLValue(d, b), ' = ', makeExpr(d, b), ')']);
  },
  function(d, b) {
    return cat(['(', makeLValue(d, b), ')', ' = ', makeExpr(d, b)]);
  },

  // Destructuring assignment
  function(d, b) {
    return cat([makeDestructuringLValue(d, b), ' = ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat([makeDestructuringLValue(d, b), ' = ', makeExpr(d, b)]);
  },
  function(d, b) {
    return cat(
        ['(', makeDestructuringLValue(d, b), ' = ', makeExpr(d, b), ')']);
  },
  function(d, b) {
    return cat(
        ['(', makeDestructuringLValue(d, b), ')', ' = ', makeExpr(d, b)]);
  },

  // Destructuring assignment with lots of group assignment
  function(d, b) {
    return cat(
        [makeDestructuringLValue(d, b), ' = ', makeDestructuringLValue(d, b)]);
  },

  // Modifying assignment, with operators that do various coercions
  function(d, b) {
    return cat([
      makeLValue(d, b), Random.index(['|=', '%=', '+=', '-=']), makeExpr(d, b)
    ]);
  },

  // ES5 getter/setter syntax, imperative (added in Gecko 1.9.3?)
  function(d, b) {
    return cat([
      'Object.defineProperty', '(', makeId(d, b), ', ', makePropertyName(d, b),
      ', ', makePropertyDescriptor(d, b), ')'
    ]);
  },

  // Test the prototype of a particular object
  function(d, b) {
    return cat(['Object.getPrototypeOf', '(', makeId(d, b), ')']);
  },
  function(d, b) {
    return cat(
        ['Object.setPrototypeOf', '(', makeId(d, b), ', ', makeId(d, b), ')']);
  },

  // Test retrieving an object's enumerable property values
  function(d, b) {
    return cat(['Object.values', '(', makeId(d, b), ')']);
  },

  // Old getter/setter syntax, imperative
  function(d, b) {
    return cat([
      makeExpr(d, b), '.', '__defineGetter__', '(', uneval(makeId(d, b)), ', ',
      makeFunction(d, b), ')'
    ]);
  },
  function(d, b) {
    return cat([
      makeExpr(d, b), '.', '__defineSetter__', '(', uneval(makeId(d, b)), ', ',
      makeFunction(d, b), ')'
    ]);
  },
  function(d, b) {
    return cat([
      'this', '.', '__defineGetter__', '(', uneval(makeId(d, b)), ', ',
      makeFunction(d, b), ')'
    ]);
  },
  function(d, b) {
    return cat([
      'this', '.', '__defineSetter__', '(', uneval(makeId(d, b)), ', ',
      makeFunction(d, b), ')'
    ]);
  },

  // Object literal
  function(d, b) {
    return cat(['(', '{', makeObjLiteralPart(d, b), ' }', ')']);
  },
  function(d, b) {
    return cat([
      '(', '{', makeObjLiteralPart(d, b), ', ', makeObjLiteralPart(d, b), ' }',
      ')'
    ]);
  },

  // Test js_ReportIsNotFunction heavily.
  function(d, b) {
    return `(p={}, (p.z = ${makeExpr(d, b)})())`;
  },

  // Test js_ReportIsNotFunction heavily.
  // Test decompilation for ".keyword" a bit.
  // Test throwing-into-generator sometimes.
  function(d, b) {
    return cat([makeExpr(d, b), '.', 'throw', '(', makeExpr(d, b), ')']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), '.', 'yoyo', '(', makeExpr(d, b), ')']);
  },

  // Test eval in various contexts. (but avoid clobbering eval)
  // Test the special "obj.eval" and "eval(..., obj)" forms.
  function(d, b) {
    return `${makeExpr(d, b)}.eval(${uneval(makeScriptForEval(d, b))})`;
  },
  function(d, b) {
    return `eval(${uneval(makeScriptForEval(d, b))})`;
  },
  function(d, b) {
    return `eval(${uneval(makeScriptForEval(d, b))}, ${makeExpr(d, b)})`;
  },

  // Uneval needs more testing than it will get accidentally.  No cat() because
  // I don't want uneval clobbered (assigned to) accidentally.
  function(d, b) {
    return `(uneval(${makeExpr(d, b)}))`;
  },

  /* eslint-disable no-multi-spaces */
  // Constructors.  No cat() because I don't want to screw with the constructors
  // themselves, just call them.
  function(d, b) {
    return `new ${Random.index(constructors)}(${makeActualArgList(d, b)})`;
  },
  function(d, b) {
    return `${Random.index(constructors)}(${makeActualArgList(d, b)})`;
  },
  /* eslint-enable no-multi-spaces */

  /* eslint-disable no-multi-spaces */
  // Unary Math functions
  function(d, b) {
    return `Math.${Random.index(unaryMathFunctions)}(${makeExpr(d, b)})`;
  },
  function(d, b) {
    return `Math.${Random.index(unaryMathFunctions)}(${makeNumber(d, b)})`;
  },
  /* eslint-enable no-multi-spaces */

  /* eslint-disable no-multi-spaces */
  // Binary Math functions
  function(d, b) {
    return `Math.${Random.index(binaryMathFunctions)}(${makeExpr(d, b)}, ${
        makeExpr(d, b)})`;
  },
  function(d, b) {
    return `Math.${Random.index(binaryMathFunctions)}(${makeExpr(d, b)}, ${
        makeNumber(d, b)})`;
  },
  function(d, b) {
    return `Math.${Random.index(binaryMathFunctions)}(${makeNumber(d, b)}, ${
        makeExpr(d, b)})`;
  },
  function(d, b) {
    return `Math.${Random.index(binaryMathFunctions)}(${makeNumber(d, b)}, ${
        makeNumber(d, b)})`;
  },
  /* eslint-enable no-multi-spaces */

  function(d, b) {
    return cat(['delete', ' ', makeId(d, b), '.', makeId(d, b)]);
  },

  // Spidermonkey: global ES5 strict mode
  function(d, b) {
    return '(void options(\'strict_mode\'))';
  },

  // Spidermonkey: additional "strict" warnings, distinct from ES5 strict mode
  function(d, b) {
    return '(void options(\'strict\'))';
  },

  // More special Spidermonkey shell functions
  // (Note: functions without returned objects or visible side effects go in
  // testing-functions.js, in order to allow presence/absence differential
  // testing.) function (d, b) { return `dumpObject(${makeExpr(d, b)})`; },
  function(d, b) {
    return `(void shapeOf(${makeExpr(d, b)}))`;
  },
  function(d, b) {
    return `intern(${makeExpr(d, b)})`;
  },
  function(d, b) {
    return 'allocationMarker()';
  },
  function(d, b) {
    return 'timeout(1800)';
  },  // see https://bugzilla.mozilla.org/show_bug.cgi?id=840284#c12 -- replace
      // when bug 831046 is fixed
  function(d, b) {
    return '(makeFinalizeObserver(\'tenured\'))';
  },
  function(d, b) {
    return '(makeFinalizeObserver(\'nursery\'))';
  },

  // makeRegexUseExpr, makeShapeyValue, makeIterable,

  function(d, b) {
    return makeMathExpr(d + rnd(3), b);
  }
];

var fuzzTestingFunctions = fuzzTestingFunctionsCtor(fuzzTestingFunctionArg);

// Ensure that even if makeExpr returns "" or "1, 2", we only pass one argument
// to functions like schedulegc (null || (" + makeExpr(d - 2, b) + ")) Darn,
// only |this| and local variables are safe: an expression with side effects
// breaks the statement-level compare_jit hack
function fuzzTestingFunctionArg(d, b) {
  return 'this';
}

function makeTestingFunctionCall(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var callStatement = Random.index(fuzzTestingFunctions.testingFunctions)(d, b);

  // Set the 'last expression evaluated' to undefined, in case we're in an eval
  // context, and the function throws in one run but not in another.
  var callBlock = `{ void 0; ${callStatement} }`;

  if (jsshell && rnd(5) === 0) {
    // Differential testing hack!
    // The idea here: make compare_jit tell us when functions like gc() surprise
    // us with visible side effects.
    // * Functions in testing-functions.js are chosen to be ones with no visible
    //   side effects except for return values (voided) or throwing (caught).
    // * This condition is controlled by --no-asmjs, which compare_jit flips.
    //     (A more principled approach would be to have compare_jit set an
    //     environment variable and read it here using os.getenv(), but os is
    //     not available when running with --fuzzing-safe...)
    // * The extra braces prevent a stray "else" from being associated with this
    // "if".
    // * The 'void 0' at the end ensures the last expression-statement is
    // consistent (needed because |eval| returns that as its result)
    var cond = `${rnd(2) ? '!' : ''}false`;
    return `{ if (${cond}) ${callBlock} void 0; }`;
  }

  return callBlock;
}

// SpiderMonkey shell (but not xpcshell) has an "evalcx" function and a
// "newGlobal" function. This tests sandboxes and cross-compartment wrappers.
if (typeof evalcx === 'function') {
  exprMakers = exprMakers.concat([
    function(d, b) {
      return makeGlobal(d, b);
    },
    function(d, b) {
      return `evalcx(${uneval(makeScriptForEval(d, b))}, ${makeExpr(d, b)})`;
    },
    function(d, b) {
      return `evalcx(${uneval(makeScriptForEval(d, b))}, ${makeGlobal(d, b)})`;
    }
  ]);
}

// SpiderMonkey shell has an "evalInWorker" function.
// This tests evaluating scripts in a separate thread with its own runtime.
if (typeof evalInWorker === 'function') {
  exprMakers = exprMakers.concat([
    function(d, b) {
      return makeGlobal(d, b);
    },
    function(d, b) {
      return `evalInWorker(${uneval(makeScriptForEval(d, b))})`;
    },
    function(d, b) {
      return `evalInWorker(${uneval(makeScriptForEval(d, b))})`;
    }
  ]);
}

// xpcshell (but not SpiderMonkey shell) has some XPC wrappers available.
if (typeof XPCNativeWrapper === 'function') {
  exprMakers = exprMakers.extend([
    function(d, b) {
      return `new XPCNativeWrapper(${makeExpr(d, b)})`;
    },
    function(d, b) {
      return `new XPCSafeJSObjectWrapper(${makeExpr(d, b)})`;
    }
  ]);
}

function makeNewGlobalArg(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  // Make an options object to pass to the |newGlobal| shell builtin.
  var propStrs = [];
  if (rnd(2)) {
    propStrs.push(`newCompartment: ${makeBoolean(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`sameCompartmentAs: ${makeExpr(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`sameZoneAs: ${makeExpr(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`cloneSingletons: ${makeBoolean(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`disableLazyParsing: ${makeBoolean(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`invisibleToDebugger: ${makeBoolean(d - 1, b)}`);
  }
  if (rnd(2)) {
    propStrs.push(`deferredParserAlloc: ${makeBoolean(d - 1, b)}`);
  }
  return `{ ${propStrs.join(', ')} }`;
}

function makeGlobal(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(10)) {
    return 'this';
  }

  var gs;
  switch (rnd(4)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      gs = 'evalcx(\'\')';
      break;
    case 1:
      gs = 'evalcx(\'lazy\')';
      break;
    default:
      gs = `newGlobal(${makeNewGlobalArg(d - 1, b)})`;
      break;
      /* eslint-enable no-multi-spaces */
  }

  if (rnd(2)) {
    gs = `fillShellSandbox(${gs})`;
  }

  return gs;
}

function makeShapeyConstructor(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);
  var argName = uniqueVarName();
  var t = rnd(4) ? 'this' : argName;
  var funText = `function shapeyConstructor(${argName}){${directivePrologue()}`;
  var bp = b.concat([argName]);

  var nPropNames = rnd(6) + 1;
  var propNames = [];
  for (var i = 0; i < nPropNames; ++i) {
    propNames[i] = makePropertyName(d, b);
  }

  var nStatements = rnd(11);
  for (var j = 0; j < nStatements; ++j) {
    var propName = Random.index(propNames);
    var tprop = `${t}[${propName}]`;
    if (rnd(5) === 0) {
      funText += `if (${rnd(2) ? argName : makeExpr(d, bp)}) `;
    }
    switch (rnd(8)) {
      /* eslint-disable no-multi-spaces */
      case 0:
        funText += `delete ${tprop};`;
        break;
      case 1:
        funText += `Object.defineProperty(${t}, ${
            rnd(2) ?
                propName :
                makePropertyName(d, b)}, ${makePropertyDescriptor(d, bp)});`;
        break;
      case 2:
        funText += `{ ${makeStatement(d, bp)} } `;
        break;
      case 3:
        funText += `${tprop} = ${makeExpr(d, bp)};`;
        break;
      case 4:
        funText += `${tprop} = ${makeFunction(d, bp)};`;
        break;
      case 5:
        funText += `for (var ytq${uniqueVarName()} in ${t}) { }`;
        break;
      case 6:
        funText += `Object.${
            Random.index(['preventExtensions', 'seal', 'freeze'])}(${t});`;
        break;
      default:
        funText += `${tprop} = ${makeShapeyValue(d, bp)};`;
        break;
        /* eslint-enable no-multi-spaces */
    }
  }
  funText += `return ${t}; }`;
  return funText;
}

var propertyNameMakers = Random.weighted([
  {
    w: 1,
    v: function(d, b) {
      return makeExpr(d - 1, b);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return maybeNeg() + rnd(20);
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `"${maybeNeg()}${rnd(20)}"`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `new String("${maybeNeg()}${rnd(20)}")`;
    }
  },
  {
    w: 5,
    v: function(d, b) {
      return simpleSource(Random.index(specialProperties));
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return simpleSource(makeId(d - 1, b));
    }
  },
  {
    w: 5,
    v: function(d, b) {
      return simpleSource(Random.index(allMethodNames));
    }
  }
]);

function maybeNeg() {
  return rnd(5) ? '' : '-';
} /* eslint-disable-line require-jsdoc */

function makePropertyName(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  return (Random.index(propertyNameMakers))(d, b);
}

function makeShapeyConstructorLoop(
    d, b) { /* eslint-disable-line require-jsdoc */
  var a = makeIterable(d, b);
  var v = makeNewId(d, b);
  var v2 = uniqueVarName(d, b);
  var bvv = b.concat([v, v2]);
  return makeShapeyConstructor(d - 1, b) +
      `/*tLoopC*/for (let ${v} of ${a}) { ` +
      'try{' +
      `let ${v2} = ${Random.index(['new ', ''])}shapeyConstructor(${
             v}); print('EETT'); ` +
      // `print(uneval(${v2}));` +
      makeStatement(d - 2, bvv) + `}catch(e){print('TTEE ' + e); }` +
      ' }';
}

function makePropertyDescriptor(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var s = '({';

  switch (rnd(3)) {
    case 0:
      // Data descriptor. Can have 'value' and 'writable'.
      if (rnd(2)) s += `value: ${makeExpr(d, b)}, `;
      if (rnd(2)) s += `writable: ${makeBoolean(d, b)}, `;
      break;
    case 1:
      // Accessor descriptor. Can have 'get' and 'set'.
      if (rnd(2)) s += `get: ${makeFunction(d, b)}, `;
      if (rnd(2)) s += `set: ${makeFunction(d, b)}, `;
      break;
    default:
  }

  if (rnd(2)) s += `configurable: ${makeBoolean(d, b)}, `;
  if (rnd(2)) s += `enumerable: ${makeBoolean(d, b)}, `;

  // remove trailing comma
  if (s.length > 2) {
    s = s.substr(0, s.length - 2);
  }

  s += '})';
  return s;
}

function makeBoolean(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);
  switch (rnd(4)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return 'true';
    case 1:
      return 'false';
    case 2:
      return makeExpr(d - 2, b);
    default:
      var m = loopModulo();
      return `(${Random.index(b)} % ${m}${Random.index([' == ', ' != '])}${
          rnd(m)})`;
      /* eslint-enable no-multi-spaces */
  }
}

function makeObjLiteralPart(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  switch (rnd(8)) {
      // Literal getter/setter
      // Surprisingly, string literals, integer literals, and float literals are
      // also good! (See https://bugzilla.mozilla.org/show_bug.cgi?id=520696.)
    case 2:
      return cat([
        ' get ', makeObjLiteralName(d, b), maybeName(d, b), '(',
        makeFormalArgList(d - 1, b), ')', makeFunctionBody(d, b)
      ]);
    case 3:
      return cat([
        ' set ', makeObjLiteralName(d, b), maybeName(d, b), '(',
        makeFormalArgList(d - 1, b), ')', makeFunctionBody(d, b)
      ]);

    case 4:
      return `/*toXFun*/${cat([
        Random.index(['toString', 'valueOf']), ': ', makeToXFunction(d - 1, b)
      ])}`;

    default:
      return cat([makeObjLiteralName(d, b), ': ', makeExpr(d, b)]);
  }
}

function makeToXFunction(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  switch (rnd(4)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return `function() { return ${makeExpr(d, b)}; }`;
    case 1:
      return 'function() { return this; }';
    case 2:
      return makeEvilCallback(d, b);
    default:
      return makeFunction(d, b);
      /* eslint-enable no-multi-spaces */
  }
}

function makeObjLiteralName(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  switch (rnd(6)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return simpleSource(makeNumber(d, b));  // a quoted number
    case 1:
      return makeNumber(d, b);
    case 2:
      return Random.index(allPropertyNames);
    case 3:
      return Random.index(specialProperties);
    default:
      return makeId(d, b);
      /* eslint-enable no-multi-spaces */
  }
}

function makeFunction(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  d = d - 1;

  if (rnd(5) === 1) {
    return makeExpr(d, b);
  }

  if (rnd(4) === 1) {
    return Random.index(builtinFunctions);
  }

  return (Random.index(functionMakers))(d, b);
}

function maybeName(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(2) === 0) {
    return ` ${makeId(d, b)} `;
  } else {
    return '';
  }
}

function directivePrologue() { /* eslint-disable-line require-jsdoc */
  var s = '';
  /*
  if (rnd(3) === 0) {
    s += '"use strict"; ';
  }
  if (rnd(30) === 0) {
    s += '"use asm"; ';
  }
  */
  return s;
}

function makeFunctionBody(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  switch (rnd(6)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return cat([' { ', directivePrologue(), makeStatement(d - 1, b), ' } ']);
    case 1:
      return cat(
          [' { ', directivePrologue(), 'return ', makeExpr(d, b), ' } ']);
    case 2:
      return cat([' { ', directivePrologue(), 'yield ', makeExpr(d, b), ' } ']);
    case 3:
      return cat([' { ', directivePrologue(), 'await ', makeExpr(d, b), ' } ']);
    default:
      return makeExpr(d, b);  // make an "expression closure"
                              /* eslint-enable no-multi-spaces */
  }
}

function functionPrefix() { /* eslint-disable-line require-jsdoc */
  return (rnd(2) === 0 ? '' : 'async ') + 'function' +
      (rnd(2) === 0 ? '' : '*');
}

var functionMakers = [
  // Note that a function with a name is sometimes considered a statement rather
  // than an expression.

  makeFunOnCallChain, makeMathFunction, makeMathyFunRef,

  /* eslint-disable no-multi-spaces */
  // Functions and expression closures
  function(d, b) {
    var v = makeNewId(d, b);
    return cat([
      functionPrefix(), ' ', maybeName(d, b), '(', v, ')',
      makeFunctionBody(d, b.concat([v]))
    ]);
  },
  function(d, b) {
    return cat([
      functionPrefix(), ' ', maybeName(d, b), '(', makeFormalArgList(d, b), ')',
      makeFunctionBody(d, b)
    ]);
  },
  /* eslint-enable no-multi-spaces */

  // Arrow functions with one argument (no parens needed) (no destructuring
  // allowed in this form?)
  function(d, b) {
    var v = makeNewId(d, b);
    return cat([v, ' => ', makeFunctionBody(d, b.concat([v]))]);
  },

  /* eslint-disable no-multi-spaces */
  // Arrow functions with multiple arguments
  function(d, b) {
    return cat(
        ['(', makeFormalArgList(d, b), ')', ' => ', makeFunctionBody(d, b)]);
  },
  /* eslint-enable no-multi-spaces */

  // The identity function
  function(d, b) {
    return `${functionPrefix()}(q) { ${directivePrologue()}return q; }`;
  },
  function(d, b) {
    return 'q => q';
  },

  // A function that does something
  function(d, b) {
    return `${functionPrefix()}(y) { ${directivePrologue()}${
        makeStatement(d, b.concat(['y']))} }`;
  },

  // A function that computes something
  function(d, b) {
    return `${functionPrefix()}(y) { ${directivePrologue()}return ${
        makeExpr(d, b.concat(['y']))} }`;
  },

  // A generator that does something
  function(d, b) {
    return `function(y) { ${directivePrologue()}yield y; ${
        makeStatement(d, b.concat(['y']))}; yield y; }`;
  },
  function(d, b) {
    return `function*(y) { ${directivePrologue()}yield y; ${
        makeStatement(d, b.concat(['y']))}; yield y; }`;
  },

  // An async function that does something
  function(d, b) {
    return `async function (y) { ${directivePrologue()}await y; ${
        makeStatement(d, b.concat(['y']))}; await y; }`;
  },

  // An async generator that does something
  function(d, b) {
    return `async function* (y) { ${directivePrologue()}await y; ${
        makeStatement(d, b.concat(['y']))}; await y; }`;
  },
  function(d, b) {
    return `async function* (y) { ${directivePrologue()}yield y; await y; ${
        makeStatement(d, b.concat(['y']))}; yield y; await y; }`;
  },

  // A simple wrapping pattern
  function(d, b) {
    return `/*wrap1*/(${functionPrefix()}(){ ${directivePrologue()}${
        makeStatement(d, b)}return ${makeFunction(d, b)}})()`;
  },

  // Wrapping with upvar: escaping, may or may not be modified
  function(d, b) {
    var v1 = uniqueVarName();
    var v2 = uniqueVarName();
    return `/*wrap2*/(${functionPrefix()}(){ ${directivePrologue()}var ${
        v1} = ${makeExpr(d, b)}; var ${v2} = ${
        makeFunction(d, b.concat([v1]))}; return ${v2};})()`;
  },

  /* eslint-disable no-multi-spaces */
  // Wrapping with upvar: non-escaping
  function(d, b) {
    var v1 = uniqueVarName();
    return `/*wrap3*/(${functionPrefix()}(){ ${directivePrologue()}var ${
        v1} = ${makeExpr(d, b)}; (${makeFunction(d, b.concat([v1]))})(); })`;
  },
  /* eslint-enable no-multi-spaces */

  // Apply, call
  function(d, b) {
    return `(${makeFunction(d - 1, b)}).apply`;
  },
  function(d, b) {
    return `(${makeFunction(d - 1, b)}).call`;
  },

  // Bind
  function(d, b) {
    return `(${makeFunction(d - 1, b)}).bind`;
  },
  function(d, b) {
    return `(${makeFunction(d - 1, b)}).bind(${makeActualArgList(d, b)})`;
  },

  // Methods with known names
  function(d, b) {
    return cat([makeExpr(d, b), '.', Random.index(allMethodNames)]);
  },

  // Special functions that might have interesting results, especially when
  // called "directly" by things like string.replace or array.map.
  function(d, b) {
    return 'eval';
  },  // eval is interesting both for its "no indirect calls" feature and for
      // the way it's implemented in spidermonkey (a special bytecode).
  function(d, b) {
    return 'new Function';
  },  // this won't be interpreted the same way for each caller of makeFunction,
      // but that's ok
  function(d, b) {
    return `(new Function(${uneval(makeStatement(d, b))}))`;
  },
  function(d, b) {
    return 'Function';
  },  // without "new"
  function(d, b) {
    return 'decodeURI';
  },
  function(d, b) {
    return 'decodeURIComponent';
  },
  function(d, b) {
    return 'encodeURI';
  },
  function(d, b) {
    return 'encodeURIComponent';
  },
  function(d, b) {
    return 'neuter';
  },
  function(d, b) {
    return 'createIsHTMLDDA';
  },  // spidermonkey shell object like the browser's document.all
  function(d, b) {
    return 'offThreadCompileScript';
  },
  function(d, b) {
    return 'runOffThreadScript';
  },
  function(d, b) {
    return 'nukeAllCCWs';
  },
  function(d, b) {
    return 'FakeDOMObject';
  },
  function(d, b) {
    return makeShapeyConstructor(d, b);
  },
  function(d, b) {
    return Random.index(typedArrayConstructors);
  },
  function(d, b) {
    return Random.index(constructors);
  }
];

if (typeof XPCNativeWrapper === 'function') {
  functionMakers = functionMakers.concat([
    function(d, b) {
      return 'XPCNativeWrapper';
    },
    function(d, b) {
      return 'XPCSafeJSObjectWrapper';
    }
  ]);
}

if (typeof oomTest === 'function' && engine !== ENGINE_JAVASCRIPTCORE) {
  functionMakers = functionMakers.concat([function(d, b) {
    return 'oomTest';
  }]);
}

var typedArrayConstructors = [
  'Int8Array', 'Uint8Array', 'Int16Array', 'Uint16Array', 'Int32Array',
  'Uint32Array', 'Float32Array', 'Float64Array', 'Uint8ClampedArray'
];

function makeTypedArrayStatements(
    d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (d < 0) return '';

  var numViews = rnd(d) + 1;
  var numExtraStatements = rnd(d) + 1;
  var buffer = uniqueVarName();
  var bufferSize = (1 + rnd(2)) * (1 + rnd(2)) * (1 + rnd(2)) * rnd(5);
  var statements = `var ${buffer} = new ${arrayBufferType()}(${bufferSize}); `;
  var bv = b.concat([buffer]);
  for (var j = 0; j < numViews; ++j) {
    var view = `${buffer}_${j}`;
    var type = Random.index(typedArrayConstructors);
    statements += `var ${view} = new ${type}(${buffer}); `;
    bv.push(view);
    var viewZero = `${view}[0]`;
    bv.push(viewZero);
    if (rnd(3) === 0) {
      statements += `print(${viewZero}); `;
    }
    if (rnd(3)) {
      statements += `${viewZero} = ${makeNumber(d - 2, b)}; `;
    }
    bv.push(`${view}[${rnd(11)}]`);
  }
  for (var k = 0; k < numExtraStatements; ++k) {
    statements += makeStatement(d - numExtraStatements, bv);
  }
  return statements;
}

function makeNumber(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var signStr = rnd(2) ? '-' : '';

  switch (rnd(60)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return makeExpr(d - 2, b);
    case 1:
      return `${signStr}0`;
    case 2:
      return `${signStr}${rnd(1000) / 1000}`;
    case 3:
      return `${signStr}${rnd(0xffffffff) / 2}`;
    case 4:
      return `${signStr}${rnd(0xffffffff)}`;
    case 5:
      return Random.index([
        '0.1', '.2', '3', '1.3', '4.', '5.0000000000000000000000', '1.2e3',
        '1e81', '1e+81', '1e-81', '1e4', '0', '-0', '(-0)', '-1', '(-1)',
        '0x99', '033', '3.141592653589793', '3/0', '-3/0', '0/0', '0x2D413CCC',
        '0x5a827999', '0xB504F332', '(0x50505050 >> 1)',
        // Boundaries of int, signed, unsigned (near +/- 2^31, +/- 2^32)
        '0x07fffffff', '0x080000000', '0x080000001', '-0x07fffffff',
        '-0x080000000', '-0x080000001', '0x0ffffffff', '0x100000000',
        '0x100000001', '-0x0ffffffff', '-0x100000000', '-0x100000001',
        // Boundaries of double
        'Number.MIN_VALUE', '-Number.MIN_VALUE', 'Number.MAX_VALUE',
        '-Number.MAX_VALUE',
        // Boundaries of maximum safe integer
        'Number.MIN_SAFE_INTEGER', '-Number.MIN_SAFE_INTEGER', '-(2**53-2)',
        '-(2**53)', '-(2**53+2)', 'Number.MAX_SAFE_INTEGER',
        '-Number.MAX_SAFE_INTEGER', '(2**53)-2', '(2**53)', '(2**53)+2',
        // See bug 1350097
        '0.000000000000001', '1.7976931348623157e308'
      ]);
    case 6:
      return `${signStr}${Math.pow(2, rnd(66)) + (rnd(3) - 1)}`;
    default:
      return `${signStr}${rnd(30)}`;
      /* eslint-enable no-multi-spaces */
  }
}

function makeLetHead(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var items = (d > 0 || rnd(2) === 0) ? rnd(10) + 1 : 1;
  var result = '';

  for (var i = 0; i < items; ++i) {
    if (i > 0) {
      result += ', ';
    }
    result += makeLetHeadItem(d - i, b);
  }

  return result;
}

function makeLetHeadItem(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  d = d - 1;

  if (d < 0 || rnd(2) === 0) {
    return rnd(2) ? uniqueVarName() : makeId(d, b);
  } else if (rnd(5) === 0) {
    return `${makeDestructuringLValue(d, b)} = ${makeExpr(d, b)}`;
  } else {
    return `${makeId(d, b)} = ${makeExpr(d, b)}`;
  }
}

function makeActualArgList(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var nArgs = rnd(3);

  if (nArgs === 0) {
    return '';
  }

  var argList = makeExpr(d, b);

  for (var i = 1; i < nArgs; ++i) {
    argList += `, ${makeExpr(d - i, b)}`;
  }

  return argList;
}

function makeFormalArgList(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  var argList = [];

  var nArgs = rnd(5) ? rnd(3) : rnd(100);
  for (var i = 0; i < nArgs; ++i) {
    argList.push(makeFormalArg(d - i, b));
  }

  if (rnd(5) === 0) {
    // https://developer.mozilla.org/en-US/docs/JavaScript/Reference/rest_parameters
    argList.push(`...${makeId(d, b)}`);
  }

  return argList.join(', ');
}

function makeFormalArg(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(8) === 1) {
    return makeDestructuringLValue(d, b);
  }

  return makeId(d, b) + (rnd(5) ? '' : ` = ${makeExpr(d, b)}`);
}

function makeNewId(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  return Random.index(['a', 'b', 'c', 'd', 'e', 'w', 'x', 'y', 'z']);
}

function makeId(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(3) === 1 && b.length) {
    return Random.index(b);
  }

  switch (rnd(200)) {
    case 0:
      return makeTerm(d, b);
    case 1:
      return makeExpr(d, b);
    case 2:
    case 3:
    case 4:
    case 5:
      return makeLValue(d, b);
    case 6:
    case 7:
      return makeDestructuringLValue(d, b);
    case 8:
    case 9:
    case 10:
      // some keywords that can be used as identifiers in some contexts (e.g.
      // variables, function names, argument names) but that's annoying, and
      // some of these cause lots of syntax errors.
      return Random.index([
        'get', 'set', 'getter', 'setter', 'delete', 'let', 'yield', 'await',
        'of'
      ]);
    case 11:
    case 12:
    case 13:
      return `this.${makeId(d, b)}`;
    case 14:
    case 15:
    case 16:
      return makeObjLiteralName(d - 1, b);
    case 17:
    case 18:
      return makeId(d - 1, b);
    case 19:
      return ' ';  // [k, v] becomes [, v] -- test how holes are handled in
                   // unexpected destructuring
    case 20:
      return 'this';
  }

  return Random.index([
    'a', 'b', 'c', 'd', 'e', 'w', 'x', 'y', 'z', 'eval', '\u3056', 'NaN'
    // "valueOf", "toString", // e.g. valueOf getter :P // bug 381242, etc
  ]);

  // eval is interesting because it cannot be called indirectly. and maybe also
  // because it has its own opcode in jsopcode.tbl. but bad things happen if you
  // have "eval setter". so let's not put eval in this list.
}

// for..in LHS can be a single variable OR it can be a destructuring array of
// exactly two elements.
function makeForInLHS(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);
  return makeLValue(d, b);
}

function makeLValue(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (d <= 0 || (rnd(2) === 1)) {
    return makeId(d - 1, b);
  }

  d = rnd(d);  // !

  return (Random.index(lvalueMakers))(d, b);
}

var lvalueMakers = [
  // Simple variable names :)
  function(d, b) {
    return cat([makeId(d, b)]);
  },

  // Parenthesized lvalues
  function(d, b) {
    return cat(['(', makeLValue(d, b), ')']);
  },

  // Destructuring
  function(d, b) {
    return makeDestructuringLValue(d, b);
  },
  function(d, b) {
    return `(${makeDestructuringLValue(d, b)})`;
  },

  // Certain functions can act as lvalues!  See JS_HAS_LVALUE_RETURN in js
  // engine source.
  function(d, b) {
    return cat([makeId(d, b), '(', makeExpr(d, b), ')']);
  },
  function(d, b) {
    return cat(['(', makeExpr(d, b), ')', '(', makeExpr(d, b), ')']);
  },

  // Builtins
  function(d, b) {
    return Random.index(builtinProperties);
  },
  function(d, b) {
    return Random.index(builtinObjectNames);
  },

  // Arguments object, which can alias named parameters to the function
  function(d, b) {
    return 'arguments';
  },
  function(d, b) {
    return cat(['arguments', '[', makePropertyName(d, b), ']']);
  },
  function(d, b) {
    return `${makeFunOnCallChain(d, b)}.arguments`;
  },  // read-only arguments object

  // Property access / index into array
  function(d, b) {
    return cat([makeExpr(d, b), '.', makeId(d, b)]);
  },
  function(d, b) {
    return cat([makeExpr(d, b), '.', '__proto__']);
  },
  function(d, b) {
    return cat([makeExpr(d, b), '[', makePropertyName(d, b), ']']);
  },

  // Intentionally bogus, but not quite garbage.
  function(d, b) {
    return makeExpr(d, b);
  }
];

function makeDestructuringLValue(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  d = d - 1;

  if (d < 0 || rnd(4) === 1) {
    return makeId(d, b);
  }

  if (rnd(6) === 1) {
    return makeLValue(d, b);
  }

  return (Random.index(destructuringLValueMakers))(d, b);
}

var destructuringLValueMakers = [
  // destructuring assignment: arrays
  function(d, b) {
    var len = rnd(d, b);
    if (len === 0) {
      return '[]';
    }

    var Ti = [];
    Ti.push('[');
    Ti.push(maybeMakeDestructuringLValue(d, b));
    for (var i = 1; i < len; ++i) {
      Ti.push(', ');
      Ti.push(maybeMakeDestructuringLValue(d, b));
    }

    Ti.push(']');

    return cat(Ti);
  },

  // destructuring assignment: objects
  function(d, b) {
    var len = rnd(d, b);
    if (len === 0) {
      return '{}';
    }
    var Ti = [];
    Ti.push('{');
    for (var i = 0; i < len; ++i) {
      if (i > 0) {
        Ti.push(', ');
      }
      Ti.push(makeId(d, b));
      if (rnd(3)) {
        Ti.push(': ');
        Ti.push(makeDestructuringLValue(d, b));
      }  // else, this is a shorthand destructuring, treated as "id: id".
    }
    Ti.push('}');

    return cat(Ti);
  }
];

// Allow "holes".
function maybeMakeDestructuringLValue(
    d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(2) === 0) {
    return '';
  }

  return makeDestructuringLValue(d, b);
}

function makeTerm(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  return (Random.index(termMakers))(d, b);
}

var termMakers = [
  // Variable names
  function(d, b) {
    return makeId(d, b);
  },

  // Simple literals (no recursion required to make them)
  function(d, b) {
    return Random.index([
      // Arrays
      '[]', '[1]', '[[]]', '[[1]]', '[,]', '[,,]', '[1,,]',
      // Objects
      '{}', '({})', '({a1:1})',
      // Possibly-destructuring arrays
      '[z1]', '[z1,,]', '[,,z1]',
      // Possibly-destructuring objects
      '({a2:z2})', 'function(id) { return id }', 'function ([y]) { }',
      '(function ([y]) { })()',

      'arguments', 'Math', 'this', 'length',

      '"\u03A0"'  // unicode not escaped
    ]);
  },
  makeNumber,
  function(d, b) {
    return Random.index(['true', 'false', 'undefined', 'null', 'this']);
  },
  function(d, b) {
    return Random.index([' "" ', ' \'\' ']);
  },
  randomUnitStringLiteral,  // unicode escaped
  function(d, b) {
    return Random.index([' /x/ ', ' /x/g ']);
  }
  // makeRegex
];

function randomUnitStringLiteral() { /* eslint-disable-line require-jsdoc */
  var s = '"\\u';
  for (var i = 0; i < 4; ++i) {
    s += '0123456789ABCDEF'.charAt(rnd(16));
  }
  s += '"';
  return s;
}

function maybeMakeTerm(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(2)) {
    return makeTerm(d - 1, b);
  } else {
    return '';
  }
}

function makeCrazyToken() { /* eslint-disable-line require-jsdoc */
  if (rnd(3) === 0) {
    return String.fromCharCode(32 + rnd(128 - 32));
  }
  if (rnd(6) === 0) {
    return String.fromCharCode(rnd(65536));
  }

  return Random.index([

    // Some of this is from reading jsscan.h.

    // Comments; comments hiding line breaks.
    '//', UNTERMINATED_COMMENT, (`${UNTERMINATED_COMMENT}\n`), '/*\n*/',

    // groupers (which will usually be unmatched if they come from here ;)
    '[', ']', '{', '}', '(', ')',

    // a few operators
    '!', '@', '%', '^', '*', '**', '|', ':', '?', '\'', '"', ',', '.', '/', '~',
    '_', '+', '=', '-', '++', '--', '+=', '%=', '|=', '-=', '...', '=>',

    // most real keywords plus a few reserved keywords
    ' in ', ' instanceof ', ' let ', ' new ', ' get ', ' for ', ' if ',
    ' else ', ' else if ', ' try ', ' catch ', ' finally ', ' export ',
    ' import ', ' void ', ' with ', ' default ', ' goto ', ' case ', ' switch ',
    ' do ', ' /*infloop*/while ', ' return ', ' yield ', ' await ', ' break ',
    ' continue ', ' typeof ', ' var ', ' const ',

    // reserved when found in strict mode code
    ' package ',

    // several keywords can be used as identifiers. these are just a few of
    // them.
    ' enum ',      // JS_HAS_RESERVED_ECMA_KEYWORDS
    ' debugger ',  // JS_HAS_DEBUGGER_KEYWORD
    ' super ',     // TOK_PRIMARY!

    ' this ',       // TOK_PRIMARY!
    ' null ',       // TOK_PRIMARY!
    ' undefined ',  // not a keyword, but a default part of the global object
    '\n',  // trigger semicolon insertion, also acts as whitespace where it
           // might not be expected
    '\r',
    '\u2028',  // LINE_SEPARATOR?
    '\u2029',  // PARA_SEPARATOR?
    '<' +
        '!' +
        '--',  // beginning of HTML-style to-end-of-line comment (!)
    '--' +
        '>',  // end of HTML-style comment
    '',
    '\0'  // confuse anything that tries to guess where a string ends. but note:
          // "illegal character"!
  ]);
}

function makeShapeyValue(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(10) === 0) {
    return makeExpr(d, b);
  }

  var a = [
    // Numbers and number-like things
    [
      '0', '1', '2', '3', '0.1', '.2', '1.3', '4.', '5.0000000000000000000000',
      '1.2e3', '1e81', '1e+81', '1e-81', '1e4', '-0', '(-0)', '-1', '(-1)',
      '0x99', '033', '3/0', '-3/0', '0/0', 'Math.PI', '0x2D413CCC',
      '0x5a827999', '0xB504F332', '-0x2D413CCC', '-0x5a827999', '-0xB504F332',
      '0x50505050', '(0x50505050 >> 1)',

      // various powers of two, with values near JSVAL_INT_MAX especially tested
      '0x10000000', '0x20000000', '0x3FFFFFFE', '0x3FFFFFFF', '0x40000000',
      '0x40000001'
    ],

    // Boundaries
    [
      // Boundaries of int, signed, unsigned (near +/- 2^31, +/- 2^32)
      '0x07fffffff', '0x080000000', '0x080000001', '-0x07fffffff',
      '-0x080000000', '-0x080000001', '0x0ffffffff', '0x100000000',
      '0x100000001', '-0x0ffffffff', '-0x100000000', '-0x100000001',

      // Boundaries of double
      'Number.MIN_VALUE', '-Number.MIN_VALUE', 'Number.MAX_VALUE',
      '-Number.MAX_VALUE',

      // Boundaries of maximum safe integer
      'Number.MIN_SAFE_INTEGER', '-Number.MIN_SAFE_INTEGER', '-(2**53-2)',
      '-(2**53)', '-(2**53+2)', 'Number.MAX_SAFE_INTEGER',
      '-Number.MAX_SAFE_INTEGER', '(2**53)-2', '(2**53)', '(2**53)+2',

      // See bug 1350097 - 1.79...e308 is the largest (by module) finite number
      '0.000000000000001', '1.7976931348623157e308'
    ],

    // Special numbers
    ['(1/0)', '(-1/0)', '(0/0)'],

    // String literals
    [' "" ', ' \'\' ', ' \'A\' ', ' \'\\0\' ', ' "use strict" '],

    // Regular expression literals
    [' /x/ ', ' /x/g '],

    // Booleans
    ['true', 'false'],

    // Undefined and null
    ['(void 0)', 'null'],

    // Object literals
    ['[]', '[1]', '[(void 0)]', '{}', '{x:3}', '({})', '({x:3})'],

    // Variables that really should have been constants in the ecmascript spec
    ['NaN', 'Infinity', '-Infinity', 'undefined'],

    // Boxed booleans
    ['new Boolean(true)', 'new Boolean(false)'],

    // Boxed numbers
    ['new Number(1)', 'new Number(1.5)'],

    // Boxed strings
    ['new String(\'\')', 'new String(\'q\')'],

    // Fun stuff
    ['function(){}'],
    [
      '{}', '[]', '[1]', '[\'z\']', '[undefined]', 'this', 'eval', 'arguments',
      'arguments.caller', 'arguments.callee'
    ],
    ['createIsHTMLDDA()'],

    // Actual variables (slightly dangerous)
    [b.length ? Random.index(b) : 'x']
  ];

  return Random.index(Random.index(a));
}

function mixedTypeArrayElem(d, b) { /* eslint-disable-line require-jsdoc */
  while (true) {
    var s = makeShapeyValue(d - 3, b);
    if (s.length < 60) {
      return s;
    }
  }
}

function makeMixedTypeArray(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  // Pick two to five values to use as array entries.
  var q = rnd(4) + 2;
  var picks = [];
  for (var i = 0; i < q; ++i) {
    picks.push(mixedTypeArrayElem(d, b));
  }

  // Create a large array literal by randomly repeating the values.
  var c = [];
  var count = loopCount();
  for (var j = 0; j < count; ++j) {
    var elem = Random.index(picks);
    // Sometimes, especially at the beginning of arrays, repeat a single value
    // (or type) many times (This is needed for shape warmup, but not for JIT
    // warmup)
    var repeat = count === 0 ? rnd(4) === 0 : rnd(50) === 0;
    var repeats = repeat ? rnd(30) : 1;
    for (var k = 0; k < repeats; ++k) {
      c.push(elem);
    }
  }

  return `/*MARR*/[${c.join(', ')}]`;
}

function makeArrayLiteral(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (rnd(2) === 0) {
    return makeMixedTypeArray(d, b);
  }

  var elems = [];
  while (rnd(5)) elems.push(makeArrayLiteralElem(d, b));
  return `/*FARR*/[${elems.join(', ')}]`;
}

function makeArrayLiteralElem(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  switch (rnd(5)) {
    /* eslint-disable no-multi-spaces */
    case 0:
      return `...${
          makeIterable(
              d - 1,
              b)}`;  // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Spread_operator
    case 1:
      return '';  // hole
    default:
      return makeExpr(d - 1, b);
      /* eslint-enable no-multi-spaces */
  }
}

function makeIterable(d, b) { /* eslint-disable-line require-jsdoc */
  if (rnd(TOTALLY_RANDOM) === 2) return totallyRandom(d, b);

  if (d < 1) {
    return '[]';
  }

  return (Random.index(iterableExprMakers))(d, b);
}

var iterableExprMakers = Random.weighted([
  // Arrays
  {
    w: 1,
    v: function(d, b) {
      return `new Array(${makeNumber(d, b)})`;
    }
  },
  {w: 8, v: makeArrayLiteral},

  // A generator that yields once
  {
    w: 1,
    v: function(d, b) {
      return `(function() { ${directivePrologue()}yield ${
          makeExpr(d - 1, b)}; } })()`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `(function*() { ${directivePrologue()}yield ${
          makeExpr(d - 1, b)}; } })()`;
    }
  },
  // A pass-through generator
  {
    w: 1,
    v: function(d, b) {
      return `/*PTHR*/(function() { ${directivePrologue()}for (var i of ${
          makeIterable(d - 1, b)}) { yield i; } })()`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `/*PTHR*/(function*() { ${directivePrologue()}for (var i of ${
          makeIterable(d - 1, b)}) { yield i; } })()`;
    }
  },

  // An async function that awaits once
  {
    w: 1,
    v: function(d, b) {
      return `(async function() { ${directivePrologue()}await ${
          makeExpr(d - 1, b)}; } })()`;
    }
  },

  // A pass-through async generator
  {
    w: 1,
    v: function(d, b) {
      return `/*PTHR*/(async function*() { ${
          directivePrologue()}for (var i of ${
          makeIterable(d - 1, b)}) { yield i; } })()`;
    }
  },
  {
    w: 1,
    v: function(d, b) {
      return `/*PTHR*/(async function*() { ${
          directivePrologue()}for await (var i of ${
          makeIterable(d - 1, b)}) { yield i; } })()`;
    }
  },

  {w: 1, v: makeFunction}, {w: 1, v: makeExpr}
]);

function strTimes(s, n) { /* eslint-disable-line require-jsdoc */
  if (n === 0) return '';
  if (n === 1) return s;
  var s2 = s + s;
  var r = n % 2;
  var d = (n - r) / 2;
  var m = strTimes(s2, d);
  return r ? m + s : m;
}

var allMakers = [
  makeRegisterStompFunction,
  makeRegisterStompBody,
  makeMathyFunAndTest,
  makeMathyFunRef,
  makeMathFunction,
  makeMathExpr,
  makeScript,
  makeScriptBody,
  makeScriptForEval,
  makeStatement,
  makeOpaqueIdiomaticLoop,
  makeTransparentIdiomaticLoop,
  makeBranchUnstableLoop,
  makeTypeUnstableLoop,
  makeFunOnCallChain,
  makeUseRegressionTest,
  makeNamedFunctionAndUse,
  makePrintStatement,
  makeSwitchBody,
  makeLittleStatement,
  makeStatementOrBlock,
  makeExceptionyStatement,
  makeTryBlock,
  makeExpr,
  makeTestingFunctionCall,
  makeNewGlobalArg,
  makeGlobal,
  makeShapeyConstructor,
  makePropertyName,
  makeShapeyConstructorLoop,
  makePropertyDescriptor,
  makeBoolean,
  makeObjLiteralPart,
  makeToXFunction,
  makeObjLiteralName,
  makeFunction,
  makeFunctionBody,
  makeTypedArrayStatements,
  makeNumber,
  makeLetHead,
  makeLetHeadItem,
  makeActualArgList,
  makeFormalArgList,
  makeFormalArg,
  makeNewId,
  makeId,
  makeForInLHS,
  makeLValue,
  makeDestructuringLValue,
  makeTerm,
  makeCrazyToken,
  makeShapeyValue,
  makeMixedTypeArray,
  makeArrayLiteral,
  makeArrayLiteralElem,
  makeIterable
];


function totallyRandom(d, b) { /* eslint-disable-line require-jsdoc */
  d = d + (rnd(5) - 2);        // can increase!!

  var maker = Random.index(allMakers);
  var val = maker(d, b);
  if (typeof val !== 'string') {
    throw new Error('We generated something that isn\'t a string!');
  }
  return val;
}


var totalTestGen = 0;
var maxTotalTest = 1000;

function start(glob) { /* eslint-disable-line require-jsdoc */
  var fuzzSeed = Math.floor(Math.random() * Math.pow(2, 28));
  console.error(`fuzzSeed: ${fuzzSeed}`);
  Random.init(fuzzSeed);

  // Split this string across two source strings to ensure that if a
  // generated function manages to output the entire jsfunfuzz source,
  // that output won't match the grep command.
  var cookie = '/*F';
  cookie += `RC-fuzzSeed-${fuzzSeed}*/`;

  // Can be set to true if makeStatement has side effects, such as crashing, so
  // you have to reduce "the hard way".
  var dumpEachSeed = false;

  if (dumpEachSeed) {
    console.error(`${cookie}Random.init(0);`);
  }

  // mathInitFCM();

  count = 0;

  if (jsshell) {
    // If another script specified a "maxRunTime" argument, use it; otherwise,
    // run forever
    var MAX_TOTAL_TIME = (glob.maxRunTime) || (Infinity);
    var startTime = new Date();
    var lastTime;

    do {
      testOne();
      var elapsed1 = new Date() - lastTime;
      if (elapsed1 > 1000) {
        print(`That took ${elapsed1}ms!`);
      }
      lastTime = new Date();
    } while ((lastTime - startTime < MAX_TOTAL_TIME) &&
             (totalTestGen < maxTotalTest));
  } else {
    setTimeout(testStuffForAWhile, 200); /* eslint-disable-line no-undef */
  }

  function testStuffForAWhile() { /* eslint-disable-line require-jsdoc */
    for (var j = 0; j < 100; ++j) {
      testOne();
    }

    if (count % 10000 < 100) {
      printImportant(`Iterations: ${count}`);
    }

    setTimeout(testStuffForAWhile, 30); /* eslint-disable-line no-undef */
  }

  function testOne() { /* eslint-disable-line require-jsdoc */
    ++count;

    // Sometimes it makes sense to start with simpler functions:
    // var depth = ((count / 1000) | 0) & 16;
    var depth = 14;

    if (dumpEachSeed) {
      // More complicated, but results in a much shorter script, making
      // SpiderMonkey happier.
      var MTA = uneval(Random.twister.export_mta());
      var MTI = Random.twister.export_mti();
      if (MTA !== Random.lastDumpedMTA) {
        console.error(`${cookie}Random.twister.import_mta(${MTA});`);
        Random.lastDumpedMTA = MTA;
      }
      console.error(`${cookie}Random.twister.import_mti(${
          MTI}); void (makeScript(${depth}));`);
    }

    var code = makeScript(depth);

    //  if (rnd(10) === 1) {
    //    var dp = "/*infloop-deParen*/" + Random.index(deParen(code));
    //    if (dp)
    //      code = dp;
    //  }
    // console.error(`${cookie}count=${count}; tryItOut(${uneval(code)});`);

    tryItOut(code);
  }
}

function unlikelyToHang(code) { /* eslint-disable-line require-jsdoc */
  var codeL = code.replace(/\s/g, ' ');

  // Things that are likely to hang in all JavaScript engines
  return true && code.indexOf('infloop') === -1 &&
      !(codeL.match(/for.*in.*uneval/))  // can be slow to loop through the huge
                                         // string uneval(this), for example
      && !(codeL.match(/for.*for.*for/))  // nested for loops (including for..in
                                          // etc) can take a while
      && !(codeL.match(/for.*for.*gc/));
}

function whatToTest(code) {
  return {
    allowParse: true,
    allowExec: unlikelyToHang(code),
    expectConsistentOutput: false,
    expectConsistentOutputAcrossIter: false,
    expectConsistentOutputAcrossJITs: false

  };
}

const fs = require('fs');

const TEST_BUILTIN = `

function print(x) {
  dump(x);
}
function schedulegc(...) {}
function schedulezone(...) {}
function selectforgc(...) {}
function gczeal(...) {}
function gc(...) {}
function gcslice(...) {}
function verifyprebarriers(...) {}
function gcPreserveCode(...) {}
function dumpln(x) { dump(x); }
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
`;

function tryRunning(f, code, wtt) {
  /**
   * to feed the fuzzied code into our JIT pipeline, we need to wrap the code
   * into a loop to make sure it triggers the jit.
   */

  cod = `
	${TEST_BUILTIN}

  (() => {
    for (let kkkkk = 0; kkkkk < 20; kkkkk++) {
      ${code}
    }
  })();
  `;
  fs.writeFileSync(`./output/test_${totalTestGen}.js`, code);
  totalTestGen++;
}

function tryItOut(code) { /* eslint-disable-line require-jsdoc */
  // Accidentally leaving gczeal enabled for a long time would make jsfunfuzz
  // really slow.
  if (typeof gczeal === 'function') {
    gczeal(0);
  }

  // SpiderMonkey shell does not schedule GC on its own.  Help it not use too
  // much memory.
  if (count % 1000 === 0) {
    dumpln(`Paranoid GC (count=${count})!`);
  }

  var wtt = whatToTest(code);

  if (!wtt.allowParse) {
    return;
  }

  code = code.replace(/\/\*DUPTRY\d+\*\//, function(k) {
    var n = parseInt(k.substr(8), 10);
    dumpln(n);
    return strTimes('try{}catch(e){}', n);
  });

  if (jsStrictMode) {
    code = `'use strict'; ${code}`;
  }  // ES5 10.1.1: new Function does not inherit strict mode

  var f;
  try {
    f = new Function(code); /* eslint-disable-line no-new-func */
  } catch (compileError) {
    fatal(`Compiling threw: ${errorToString(compileError)}`);
    return;
  }

  if (f && wtt.allowExec && wtt.expectConsistentOutput &&
      wtt.expectConsistentOutputAcrossJITs) {
    if (code.indexOf('\n') === -1 && code.indexOf('\r') === -1 &&
        code.indexOf('\f') === -1 && code.indexOf('\0') === -1 &&
        code.indexOf('\u2028') === -1 && code.indexOf('\u2029') === -1 &&
        code.indexOf('<--') === -1 && code.indexOf('-->') === -1 &&
        code.indexOf('//') === -1) {
      // FCM cookie, lines with this cookie are used for compare_jit
      var cookie1 = '/*F';
      var cookie2 = 'CM*/';
      var nCode = code;
      // Avoid compile-time errors because those are no fun.
      // But leave some things out of function(){} because some bugs are only
      // detectable at top-level, and pure jsfunfuzz doesn't test top-level at
      // all. (This is a good reason to use compare_jit even if I'm not
      // interested in finding JIT bugs!)
      if (nCode.indexOf('return') !== -1 || nCode.indexOf('yield') !== -1 ||
          nCode.indexOf('const') !== -1 || failsToCompileInTry(nCode)) {
        nCode = `(function(){${nCode}})()`;
      }
      dumpln(`${cookie1 + cookie2} try { ${nCode} } catch(e) { }`);
    }
  }

  tryRunning(f, code, wtt);

  dumpln('');
}

function startFuzz(total) {
  maxTotalTest = total;
  start({});
}

startFuzz(100000);

module.exports = startFuzz;
