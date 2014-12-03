struct foo{
    int baz;
};

int _cc_main() {
    struct foo * bar = alloc(struct foo);
    return 0;
}
