const lhs = -5;
const rhs = 2;
let v = 0;

for (let i = 0; i < 100; i++) {
  v = lhs >>> 1;
}

dump(v);
