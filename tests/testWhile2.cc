
int foo() {
    return 0;
}

int _cc_main() {
    int x = foo();
    int y = foo();
    while (x < 11) {
        y = x + y;
        x = x + 1;
    }
    return y;
}
