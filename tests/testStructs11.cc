struct foo {
    int baz;
};

int bar(struct foo *f) {
    return f->baz;
}

int _cc_main() {
    struct foo * f = alloc(struct foo);
    f->baz = 1;
    return bar(f);
}
