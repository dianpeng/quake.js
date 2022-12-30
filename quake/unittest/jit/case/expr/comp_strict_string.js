{
  /**
   * strict string comparison is same as normal string comparison,
   * ie issued with string lexical comparison
   */

  let lhs = 'abcd';
  let rhs = 'abcf';

  for (let i = 0; i < 1000; i++) {
    no(lhs === rhs);
    yes(lhs !== rhs);
    yes(lhs === 'abcd');
  }
}
