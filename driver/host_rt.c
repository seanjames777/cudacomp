#include <stdio.h>
#include <stdlib.h>

extern int _cc_main();

void *_rt_alloc_array(int elemSize, int length) {
    char *buff = (char *)calloc(1, elemSize * length + 8);

    // TODO: alignment :)
    *((int *)&buff[0]) = length;
    *((int *)&buff[4]) = elemSize;

    return (void *)&buff[8];
}

void print_float(float x) {
    printf("%f\n", x);
}

int rt_add(int x, int y) {
    return x + y;
}

int main(int argc, char *argv[]) {
    int result = _cc_main(&result);

    printf("%d\n", result);

    return 0;
}
