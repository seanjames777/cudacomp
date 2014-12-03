int foo(int * bar) {
    return *bar;
}

int _cc_main() {
    int * baz = alloc(int);
    return foo(baz);
}
