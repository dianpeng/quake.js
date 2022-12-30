{
  /**
   * testing a bug case that we do not correctly emit phi node for loop
   * invariant which cause RA fails
   */


  let obj1 = new Object();
  let obj2 = new Object();
  let foo = function() {
    return obj1.a + obj2.b;
  };

  obj1.a = 10;
  obj2.b = 32;

  for (let i = 0; i < 1000; ++i) {

    // obj1 and obj2 are loop invariant and should be persistent cross loop
    result = foo(obj1, obj2);

    // bunch of other operation which may spill obj1/obj2
    yes([].length == 0);
    yes({'a':100}.a == 100);
    yes(100 + 20 == 120);
    yes(true);
    yes([1, 2, 3, 4].length == 4);
  }

  yes(result == 42);
}
