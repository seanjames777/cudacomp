int _cc_main() {
    int[] arr1 = alloc_array(int, 4);
    int[] arr2 = alloc_array(int, 4);

    if (arr1 == arr2)
        return 5;
    else
        return 10;
}
