// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{

  function loop1_0 () {
    
    let z = 0;
    for (let iterator_10 = 0; iterator_10 < 20; iterator_10++) {
      for (let iterator_11 in new Array(20).fill(1)) {

            z++;
       }
     }

    
    yes(z === 400);
  }

  function loop1_1 () {
    
    let z = 0;
    for (let iterator_12 = 0; iterator_12 < 20; iterator_12++) {
      for (let iterator_13 in [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]) {

            z++;
       }
     }

    
    yes(z === 400);
  }

  function loop1_2 () {
    
    let z = 0;
    for (let iterator_14 = 0; iterator_14 < 20; iterator_14++) {
      for (let iterator_15 in [{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}]) {

            z++;
       }
     }

    
    yes(z === 400);
  }

  function loop1_3 () {
    
    let z = 0;
    for (let iterator_16 = 0; iterator_16 < 20; iterator_16++) {
      for (let iterator_17 in ['hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world']) {

            z++;
       }
     }

    
    yes(z === 400);
  }
  loop1_0();
  loop1_1();
  loop1_2();
  loop1_3();

}
// ----------------------------------------------------------------