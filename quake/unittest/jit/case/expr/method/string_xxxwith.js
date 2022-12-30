{
  let string = "abccba";
  let head = "abc";
  let tail = "cba";

  for (let i = 0; i < 10000; ++i) {
    yes(string.endsWith(tail));
    yes(string.startsWith(head));

    yes("abccba".endsWith(tail));
    yes("abccba".startsWith(head));

    yes(string.endsWith("cba"));
    yes(string.startsWith("abc"));

    yes("abc".startsWith('abc'));
    yes("cba".endsWith("cba"));
  }
}
