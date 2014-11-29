int _cc_main() {
    int sum = 0;

    for (int x : 0 to 10) {
        for (int y : 0 to 10) {
            sum = sum + (y * 10) + x;
        }
    }

    return sum;
}
