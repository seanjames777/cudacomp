struct foo {
    int bar;
};

struct foo bar(struct foo * baz) {
    return *baz;
}

int _cc_main() {
    return 0;
}
