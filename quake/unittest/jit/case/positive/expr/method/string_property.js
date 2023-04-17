{
  let str0 = "";
  let str1 = "a";
  let str2 = "abcd";
  let str3 = "a,b,c,d";

  for (let i = 0; i < 1000; i++) {
    yes(str0.length == 0);
    yes(str1.length == 1);
    yes(str2.length == 4);
    yes(str3.split(",").length == 4);
    yes(str3.split(",")[0] == "a");
  }
}
