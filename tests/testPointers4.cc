int * foo(int * y) {
    return *y;
}

int _cc_main() {
    int * cat = alloc(int);
    return *cat;
}
