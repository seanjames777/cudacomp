struct foo {
    int bar;
};

struct bar {
    struct foo foo;
};

int _cc_main() {
    // TODO
    /*struct foo *foo = alloc(struct foo);
    (*foo).bar = 5;
    struct bar *bar = alloc(struct bar);
    (*bar).foo = *foo;
    return (*bar).foo.bar;*/

    return 5;
}
