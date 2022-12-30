const jitObj = {
  "a" : 20,
  "b" : 1,
  "c" : 21,

  "answer" : function() {
    for (let i = 0; i < 100; ++i) {
      yes(this.a + this.b + this.c == 42);
    }
  }
}

{
  jitObj.answer();
}
