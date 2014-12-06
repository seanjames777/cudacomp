struct foo {
    int x;
};

struct bar {
    struct foo t;
};

int _cc_main() {
    struct bar *t = alloc(struct bar);
    t->t;
    return 0;
}
