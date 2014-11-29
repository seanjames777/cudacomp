void add(int x, int * y) {
    if (x > 0) {
        *y = *y + 1;
        add(x-1,y);
    }
}

int _cc_main() {
    int * var = alloc(int);
    add(10,var);
    return *var;
}
