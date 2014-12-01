#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

extern int _cc_main();

void *_rt_alloc_array(int elemSize, int length) {
    char *buff = (char *)calloc(1, elemSize * length + 8);

    *((int *)&buff[0]) = length;
    *((int *)&buff[4]) = elemSize;

    return (void *)&buff[8];
}

void _rt_div_check(int n, int d) {
    if (d == 0 || (n == 0x08000000 && d == 0xFFFFFFFF)) {
        // TODO
        raise(SIGKILL);
    }
}

// Used for our own testing

void print_float(float x) {
    printf("%f\n", x);
}

int rt_add(int x, int y) {
    return x + y;
}
