{
  let str0 = "abcdef";
  let str1 = "ABCD"; 

  for (let i = 0; i < 10000; ++i) {
    yes(str0.concat(str1) == "abcdefABCD");
    yes(str0.concat("ABCD") == "abcdefABCD");
    yes("abcdef".concat(str1) == "abcdefABCD");
    yes("abcdef".concat("ABCD") == "abcdefABCD");
  }

}
