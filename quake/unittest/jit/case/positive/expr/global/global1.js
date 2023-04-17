global_1 = 20;
global_2 = 2;
global_3 = 20;
global_d = "hello world: ";

{
  let result = "";
  for (let i = 0; i < 15; i++) {
    result = global_d + (global_1 + global_2 + global_3);
  }
  yes(result == "hello world: 42");
}
