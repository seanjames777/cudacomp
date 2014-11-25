int fib(int n) {
    int[] arr = alloc_array(int, n + 1);

    arr[0] = 0;
    arr[1] = 1;

    int i = 2;

    while (i <= n) {
        arr[i] = arr[i - 1] + arr[i - 2];
        i = i + 1;
    }

    return arr[n];
}

int _cc_main() {
    return fib(9);
}
