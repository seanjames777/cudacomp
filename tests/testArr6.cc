extern int[] rt_calloc(int num, int size);

int _cc_main() {
    int[] arr1 = rt_calloc(4, 4);

    if (arr1 == arr1)
        return 5;
    else
        return 10;
}
