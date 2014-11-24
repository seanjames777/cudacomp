extern int[] rt_calloc(int num, int size);

int _cc_main() {
    int[] arr = rt_calloc(4, 4);

    return arr[0];
}
