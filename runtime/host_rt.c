#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

extern int _cc_main();

void *_rt_alloc_array(int elemSize, int length) {
    if (length < 0)
        raise(SIGSEGV);

    char *buff = (char *)calloc(1, elemSize * length + 8);

    *((int *)&buff[0]) = length;

    return (void *)&buff[8];
}

void _rt_array_bounds_check(char *array, int idx) {
    if (!array)
        raise(SIGSEGV);

    int len = *((int *)&array[-8]);

    // Handles negative as well
    if ((unsigned int)idx >= len)
        raise(SIGSEGV);
}

void _rt_deref_check(char *ptr) {
    if (!ptr)
        raise(SIGSEGV);
}

void *_rt_alloc(int size) {
    return (void*)calloc(1, size);
}

void _rt_div_check(int n, int d) {
    if (d == 0 || (n == 0x80000000 && d == 0xFFFFFFFF))
        raise(SIGFPE);
}

void _rt_shift_check(int s) {
    // Handles negative as well
    if ((unsigned int)s >= 32)
        raise(SIGFPE);
}

void _rt_assert(int c) {
    if (!c)
        raise(SIGABRT); // TODO: call abort()?
}

// Used for our own testing
void print_float(float x) {
    printf("%f\n", x);
}

int rt_add(int x, int y) {
    return x + y;
}
