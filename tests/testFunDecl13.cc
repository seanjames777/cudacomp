int bigFunc() {
    int x = 0;
    int y = 1;
    int z = 2;

    while (x < 10) {
        x = x + 1;
        y = y << 1;
        z = z * z;
    }

    return x + y + z;
}

int helper() {
    return bigFunc() / 2;
}

int _cc_main() {
    return helper() + 1;
}
