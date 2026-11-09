// D-04 canary fixture: one hot loop (default backedge threshold is 10, so
// 20000 trips is comfortably traced and compiled) computing an integer sum.
//
// Notes, the increment is deliberately `sum += 2` and NOT `sum += 1`: the
// ENABLE_TEST injection corrupts EVERY integer-add result by +1, including
// the induction `i++`. With `sum += 1` the two corruptions cancel out
// exactly (10000 iterations x 2 == 20000 x 1 == the correct sum) and the
// armed canary would silently pass. With `sum += 2` the armed jit run
// computes 10000 iterations x 3 = 30000 against the oracle's 40000.
{
  let sum = 0;
  for (let i = 0; i < 20000; i++) {
    sum += 2;
  }
  dump(sum);
  yes(sum === 40000);
  dump(sum === 40000);
}
