function foo(i) {}

{
  /**
   * this case never gets JITTed for now since the i % 2 is an unbiased
   * branch, which will bunce between deoptimize and optimize every other
   * loop. This is fine, we just testify whether the continue works or not
   */

  let sum = 0;
  for (let i = 0; i < 100; ++i) {
    foo(i);
    if (i % 2) continue;
    sum += i;
  }

  yes(sum == 2450);
}
