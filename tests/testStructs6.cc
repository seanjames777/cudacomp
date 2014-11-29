struct foo {
    int bar;
};

struct bar {
    int bar;
    struct foo foo;
};

struct baz {
    struct bar bar;
};

int _cc_main() {
    struct foo *foo = alloc(struct foo);
    (*foo).bar = 5;
    struct bar *bar = alloc(struct bar);
    (*bar).bar = 5;
    (*bar).foo = *foo;
    struct baz *baz = alloc(struct baz);
    (*baz).bar = *bar; 

    return (*((*baz).bar)).bar +
           ((*((*baz).bar)).foo).bar;
}
