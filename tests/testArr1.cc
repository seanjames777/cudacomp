extern int[] malloc(int size);

void func(int[] arr) {
}

int _cc_main() {
    int[] arr = malloc(4 * 4);
    func(arr);

    return 5;
}
