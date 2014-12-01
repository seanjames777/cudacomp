struct foo {
    int foo;
};

struct bar {
    struct foo c;
};

struct baz {
    struct bar *t;
};

struct buzz {
    struct baz t1;
    struct baz *t2;
};

int _cc_main() {
    struct foo *foo_ptr = alloc(struct foo);
    foo_ptr->foo = 5;
    struct bar *veryfoo = alloc(struct bar);
    veryfoo->c.foo = foo_ptr->foo;
    struct baz *notfoo = alloc(struct baz);
    notfoo->t = veryfoo;
    struct buzz *bird = alloc(struct buzz);
    bird->t2 = notfoo;
    bird->t1.t = notfoo->t;
    foo_ptr->foo = 10;

    return bird->t1.t->c.foo + bird->t2->t->c.foo;
}
