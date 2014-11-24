extern int[] calloc(int num, int size);

int _cc_main() {
    int[] arr1 = calloc(4, 4);

    if (arr1 == 5)
        return 5;

    return 10;
}
