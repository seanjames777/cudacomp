extern int[] rt_calloc(int num, int size);

int _cc_main() {
    int[] arr1 = rt_calloc(4, 4);

    arr1[0] = true;

    return 0;
}
