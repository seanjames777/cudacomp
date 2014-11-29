struct foo {
    int bar;
};

int _cc_main() {
    struct foo *baz = alloc(struct foo);
    (*baz).bar = 1;
    return (*baz).bar;
}
