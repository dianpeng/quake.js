// this program is a JavaScript version of Mersenne Twister, with concealment
// and encapsulation in class, an almost straight conversion from the original
// program, mt19937ar.c, translated by y. okada on July 17, 2006.

// Changes by Jesse Ruderman:
//   * Use intish/int32 rather than uint32 for intermediate calculations
//     (see https://bugzilla.mozilla.org/show_bug.cgi?id=883748#c1)
//   * Added functions for exporting/importing the entire PRNG state
//   * Removed parts not needed for fuzzing

// in this program, procedure descriptions and comments of original source code
// were not removed. lines commented with //c// were originally descriptions of
// c procedure. and a few following lines are appropriate JavaScript
// descriptions. lines commented with /* and */ are original comments. lines
// commented with // are additional comments in this JavaScript version. before
// using this version, create at least one instance of MersenneTwister19937
// class, and initialize the each state, given below in c comments, of all the
// instances.

/* exported MersenneTwister19937 */

/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

function MersenneTwister19937() { /* eslint-disable-line require-jsdoc */
  const N = 624;
  const M = 397;
  const MAG01 = new Int32Array([0, 0x9908b0df]);

  var mt = new Int32Array(N); /* the array for the state vector */
  var mti = 625;

  this.seed = function(s) {
    mt[0] = s | 0;
    for (mti = 1; mti < N; mti++) {
      mt[mti] = Math.imul(1812433253, mt[mti - 1] ^ (mt[mti - 1] >>> 30)) + mti;
    }
  };

  /* eslint-disable camelcase */
  this.export_state = function() {
    return [mt, mti];
  };
  this.import_state = function(s) {
    mt = s[0];
    mti = s[1];
  };
  this.export_mta = function() {
    return mt;
  };
  this.import_mta = function(_mta) {
    mt = _mta;
  };
  this.export_mti = function() {
    return mti;
  };
  this.import_mti = function(_mti) {
    mti = _mti;
  };
  /* eslint-enable camelcase */

  function mag01(y) { /* eslint-disable-line require-jsdoc */
    return MAG01[y & 0x1];
  }

  this.int32 = function() {
    var y;
    var kk;

    if (mti >= N) { /* generate N words at one time */
      for (kk = 0; kk < N - M; kk++) {
        y = ((mt[kk] & 0x80000000) | (mt[kk + 1] & 0x7fffffff));
        mt[kk] = (mt[kk + M] ^ (y >>> 1) ^ mag01(y));
      }
      for (; kk < N - 1; kk++) {
        y = ((mt[kk] & 0x80000000) | (mt[kk + 1] & 0x7fffffff));
        mt[kk] = (mt[kk + (M - N)] ^ (y >>> 1) ^ mag01(y));
      }
      y = ((mt[N - 1] & 0x80000000) | (mt[0] & 0x7fffffff));
      mt[N - 1] = (mt[M - 1] ^ (y >>> 1) ^ mag01(y));
      mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y = y ^ (y >>> 11);
    y = y ^ ((y << 7) & 0x9d2c5680);
    y = y ^ ((y << 15) & 0xefc60000);
    y = y ^ (y >>> 18);

    return y;
  };
}

var Random = {
  twister: null,

  init: function(seed) {
    if (seed == null || seed === undefined) {
      seed = new Date().getTime();
    }
    this.twister = new MersenneTwister19937();
    this.twister.seed(seed);
  },
  number: function(limit) {
    // Returns an integer in [0, limit). Uniform distribution.
    if (limit === 0) {
      return limit;
    }
    if (limit == null || limit === undefined) {
      limit = 0xffffffff;
    }
    return (Random.twister.int32() >>> 0) % limit;
  },
  float: function() {
    // Returns a float in [0, 1]. Uniform distribution.
    return (Random.twister.int32() >>> 0) * (1.0 / 4294967295.0);
  },
  range: function(start, limit) {
    // Returns an integer in [start, limit]. Uniform distribution.
    if (isNaN(start) || isNaN(limit)) {
      Utils.traceback();
      throw new TypeError(
          `Random.range() received a non number type: '${start}', '${limit}')`);
    }
    return Random.number(limit - start + 1) + start;
  },
  ludOneTo: function(limit) {
    // Returns a float in [1, limit]. The logarithm has uniform distribution.
    return Math.exp(Random.float() * Math.log(limit));
  },
  index: function(list, emptyr) {
    if (!(list instanceof Array ||
          (typeof list !== 'string' && 'length' in list))) {
      Utils.traceback();
      throw new TypeError(
          `Random.index() received a non array type: '${list}'`);
    }
    if (!list.length) {
      return emptyr;
    }
    return list[this.number(list.length)];
  },
  key: function(obj) {
    var list = [];
    for (var i in obj) {
      list.push(i);
    }
    return this.index(list);
  },
  bool: function() {
    return this.index([true, false]);
  },
  pick: function(obj) {
    if (typeof obj === 'function') {
      return obj();
    }
    if (obj instanceof Array) {
      return this.pick(this.index(obj));
    }
    return obj;
  },
  chance: function(limit) {
    if (limit == null || limit === undefined) {
      limit = 2;
    }
    if (isNaN(limit)) {
      Utils.traceback();
      throw new TypeError(
          `Random.chance() received a non number type: '${limit}'`);
    }
    return this.number(limit) === 1;
  },
  choose: function(list, flat) {
    if (!(list instanceof Array)) {
      Utils.traceback();
      throw new TypeError(
          `Random.choose() received a non-array type: '${list}'`);
    }
    var total = 0;
    for (var i = 0; i < list.length; i++) {
      total += list[i][0];
    }
    var n = this.number(total);
    for (let j = 0; j < list.length; j++) {
      if (n < list[j][0]) {
        if (flat === true) {
          return list[j][1];
        } else {
          return this.pick([list[j][1]]);
        }
      }
      n = n - list[j][0];
    }
    if (flat === true) {
      return list[0][1];
    }
    return this.pick([list[0][1]]);
  },
  weighted: function(wa) {
    // More memory-hungry but hopefully faster than Random.choose$flat
    var a = [];
    for (var i = 0; i < wa.length; ++i) {
      for (var j = 0; j < wa[i].w; ++j) {
        a.push(wa[i].v);
      }
    }
    return a;
  },
  use: function(obj) {
    return Random.bool() ? obj : '';
  },
  shuffle: function(arr) {
    var len = arr.length;
    var i = len;
    while (i--) {
      var p = Random.number(i + 1);
      var t = arr[i];
      arr[i] = arr[p];
      arr[p] = t;
    }
  },
  shuffled: function(arr) {
    var newArray = arr.slice();
    Random.shuffle(newArray);
    return newArray;
  },
  subset: function(a) {
    // TODO: shuffle, repeat, include bogus things [see also
    // https://github.com/mozilla/rust/blob/d0ddc69298c41df04b0488d91d521eb531d79177/src/fuzzer/ivec_fuzz.rs]
    // Consider adding a weight argument, or swarming on inclusion/exclusion to
    // make 'all' and 'none' more likely
    var subset = [];
    for (var i = 0; i < a.length; ++i) {
      if (rnd(2)) {
        subset.push(a[i]);
      }
    }
    return subset;
  }

};

function rnd(n) {
  return Random.number(n);
} /* eslint-disable-line require-jsdoc */

module.exports = {
  rnd: rnd,
  Random: Random
}
