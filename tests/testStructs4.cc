struct foo {
    int bar;
    struct foo* f;
};

int _cc_main() {
    struct foo *baz = alloc(struct foo);
    baz->bar = 5;
    baz->f = baz;
    return baz->f->bar;
}
