// matrix multiplication
function randMat(length, start) {
  const m = [];
  for (let i = 0; i < length; ++i) {
    const raw = [];
    for (let j = 0; j < length; ++j) {
      raw.push(start + j);
    }
    m.push(raw);
  }
  return m;
}

function matMul(lhs, rhs) {
  const n = lhs.length;
  const out = [];
  for (let i = 0; i < n; ++i) {
    const raw = [];
    for (let j = 0; j < n; j++) {
      let value = 0;
      for (let k = 0; k < n; k++) {
        const l = lhs[i][k];
        const r = rhs[k][j];
        value += l * r;
      }
      raw.push(value);
    }
    out.push(raw);
  }
  return out;
}

function printMat(mat) {
  for (let i = 0; i < mat.length; i++) {
    const r = mat[i];
    let out = `${i}=> `;
    for (let j = 0; j < r.length; j++) {
      out += `${r[j]};`;
    }
    dump(out);
  }
}

{
  let start = now();
  const mat1 = randMat(500, 1);
  const mat2 = randMat(500, 20);
  const out = matMul(mat1, mat2);
  let end = now();

  dump("(matmul: 500 * 500) time: " + (end - start));
}

// this is hardcoded verification case
{
  const mat1 = randMat(15, 1);
  const mat2 = randMat(15, 20);
  const out = matMul(mat1, mat2);
  const raw = [
    2400,
    2520,
    2640,
    2760,
    2880,
    3000,
    3120,
    3240,
    3360,
    3480,
    3600,
    3720,
    3840,
    3960,
    4080
  ];

  yes(out.length == 15);
  for (let i = 0; i < 15; i++) {
    const r = out[i];
    yes(r.length === 15);
    for (let j = 0; j < 15; j++) {
      yes(r[j] === raw[j]);
    }
  }
  // verify the output here
}
