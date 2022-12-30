
const Obj = {
  "a" : 21,
  "b" : 21,

  "answer": function() {
    return this.a + this.b;
  },

  "newLife": function(foo) {
    return this.answer() + foo;
  }
};

{
  for (let i = 0; i < 1000; i++) {
    yes(Obj.answer());
    yes(Obj.newLife(42) == 42 + 42);
  }
}
