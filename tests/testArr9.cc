extern int[] calloc(int num, int size);

int _cc_main() {
    int[] arr1 = calloc(4, 4);

    arr1[true] = 5;

    return 0;
}
