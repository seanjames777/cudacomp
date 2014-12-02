void par_add<<<int i>>>(int[] a, int[] b, int[] c) {
    c[i] = a[i] + b[i];
}

int _cc_main() {
    int[] arr1 = alloc_array(int, 5);
    int[] arr2 = alloc_array(int, 5);
    int[] arr3 = alloc_array(int, 5);

    int i = 0;

    while (i < 5) {
        arr1[i] = i;
        arr2[i] = i;
        i++;
    }

    @device
    for (int j : 0 to 5)
        par_add<<<j>>>(arr1, arr2, arr3);

    i = 0;

    while (i < 5) {
        if (arr3[i] != arr1[i] + arr2[i])
            return 0;
        i++;
    }

    return 1;
}
