/**
 * home object, ie the super
 */

{
  class Base {
    constructor() {
      this.a = 1;
      this.b = 2;
    }
  }

  class Sub extends Base {
    constructor() {
      super();
      this.c = 3;
    }
  }

  {
    let value_a = undefined;
    let value_b = undefined;
    let value_c = undefined;

    for (let i = 0; i < 15; i++) {
      const s = new Sub();
      value_a = s.a;
      value_b = s.b;
      value_c = s.c;
    }
    yes(value_a == 1);
    yes(value_b == 2);
    yes(value_c == 3);
  }
}


{
  class Base {
    constructor() {
      this.a = 1;
      this.b = 2;
    }
  }

  class Sub extends Base {
    constructor() {
      super();
      this.a = 10;
      this.b = 20;
      this.c = 3;
    }
  }


  {
    let value_a = undefined;
    let value_b = undefined;
    let value_c = undefined;

    for (let i = 0; i < 15; i++) {
      const s = new Sub();
      value_a = s.a;
      value_b = s.b;
      value_c = s.c;
    }
    yes(value_a == 10);
    yes(value_b == 20);
    yes(value_c == 3);
  }
}

{
  class Base {
    constructor(a, b) {
      this.a = a;
      this.b = b;
    }
  }

  class Sub extends Base {
    constructor() {
      super(100, 200);
      this.c = 3;
    }
  }


  {
    let value_a = undefined;
    let value_b = undefined;
    let value_c = undefined;

    for (let i = 0; i < 15; i++) {
      const s = new Sub();
      value_a = s.a;
      value_b = s.b;
      value_c = s.c;
    }
    yes(value_a == 100);
    yes(value_b == 200);
    yes(value_c == 3);
  }
}
