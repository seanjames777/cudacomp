extern int[] rt_calloc(int num, int size);

void func(int[] arr) {
}

int _cc_main() {
    int[] arr = rt_calloc(4, 4);
    func(arr);

    return 5;
}
