int fib(int n) {
    int result;
    if (n == 0)
        result = 0;
    else if (n == 1)
        result = 1;
    else
        result = fib(n - 2) + fib(n - 1);

    return result;
}

int _cc_main() {
    return fib(9);
}
