// Test case is generated, DO NOT MODIFY!!!
// --------------------------------------------------------------
{

  function loop_mixed4_0 () {
    
    let z = 0;
    for (let iterator_741 of new Array(20).fill(1)) {
      for (let iterator_742 = 0; iterator_742 < 20; iterator_742++) {
        for (let iterator_743 = 0; iterator_743 < 20; iterator_743++) {

            z++;
         }
       }
     }

    
    yes(z === 20*20*20);
  }

  function loop_mixed4_1 () {
    
    let z = 0;
    for (let iterator_744 of [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]) {
      for (let iterator_745 = 0; iterator_745 < 20; iterator_745++) {
        for (let iterator_746 = 0; iterator_746 < 20; iterator_746++) {

            z++;
         }
       }
     }

    
    yes(z === 20*20*20);
  }

  function loop_mixed4_2 () {
    
    let z = 0;
    for (let iterator_747 of [{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}]) {
      for (let iterator_748 = 0; iterator_748 < 20; iterator_748++) {
        for (let iterator_749 = 0; iterator_749 < 20; iterator_749++) {

            z++;
         }
       }
     }

    
    yes(z === 20*20*20);
  }

  function loop_mixed4_3 () {
    
    let z = 0;
    for (let iterator_750 of ['hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world','hello world']) {
      for (let iterator_751 = 0; iterator_751 < 20; iterator_751++) {
        for (let iterator_752 = 0; iterator_752 < 20; iterator_752++) {

            z++;
         }
       }
     }

    
    yes(z === 20*20*20);
  }
  loop_mixed4_0();
  loop_mixed4_1();
  loop_mixed4_2();
  loop_mixed4_3();

}
// ----------------------------------------------------------------