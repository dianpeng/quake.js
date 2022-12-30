{
  let i8_arr = new Int8Array(8);
  i8_arr[5] = 4;

  let u8_arr = new Uint8Array(8);
  u8_arr[5] = 4;

  let i16_arr = new Int16Array(8);
  i16_arr[5] = 4;

  let u16_arr = new Uint16Array(8);
  u16_arr[5] = 4;

  let i32_arr = new Int32Array(8);
  i32_arr[5] = 4;

  let u32_arr = new Uint32Array(8);
  u32_arr[5] = 4;

  let f32_arr = new Float32Array(8);
  f32_arr[5] = 4;

  let f64_arr = new Float64Array(8);
  f64_arr[5] = 4;

  for (let i = 0; i < 1000; i++) {
    yes(i8_arr[5] == 4);
    yes(i8_arr[10] == undefined);

    yes(i16_arr[5] == 4);
    yes(i16_arr[10] == undefined);

    yes(i32_arr[5] == 4);
    yes(i32_arr[10] == undefined);

    yes(u8_arr[5] == 4);
    yes(u8_arr[10] == undefined);

    yes(u16_arr[5] == 4);
    yes(u16_arr[10] == undefined);

    yes(u32_arr[5] == 4);
    yes(u32_arr[10] == undefined);

    yes(f32_arr[5] == 4);
    yes(f32_arr[10] == undefined);

    yes(f64_arr[5] == 4);
    yes(f64_arr[10] == undefined);
  }
}
