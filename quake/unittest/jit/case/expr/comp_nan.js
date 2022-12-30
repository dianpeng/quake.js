/** NaN number comparison **/

{
  let result = [];
  let nan = NaN;
  let zero= 0;

  for (let i = 0; i < 10000; ++i) {
    result[0] = nan == zero;
    result[1] = nan != zero;
    result[2] = nan < zero;
    result[3] = nan <= zero;
    result[4] = nan > zero;
    result[5] = nan >= zero;
  }

  no(result[0]);
  yes(result[1]);
  no(result[2]);
  no(result[3]);
  no(result[4]);
  no(result[5]);
}
