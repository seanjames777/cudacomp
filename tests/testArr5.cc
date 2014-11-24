extern int[] calloc(int num, int size);

int _cc_main() {
    int[] arr1 = calloc(4, 4);
    int[] arr2 = calloc(4, 4);

    if (arr1 == arr2)
        return 5;
    else
        return 10;
}
