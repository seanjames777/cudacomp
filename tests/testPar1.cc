void par_add<<<int i>>>(int[] a, int[] b, int[] c) {
    c[i] = a[i] + b[i];
}

int _cc_main() {
    int[] arr1 = alloc_array(int, 5);
    int[] arr2 = alloc_array(int, 5);
    int[] arr3 = alloc_array(int, 5);

    for (int i = 0; i < 5; i++) {
        arr1[i] = i;
        arr2[i] = 2 * i;
    }

    par_add<<<5>>>(arr1, arr2, arr3);

    for (int i = 0; i < 5; i++)
        if (arr3[i] != arr1[i] + arr2[i])
            return -1;

    return 0;
}
