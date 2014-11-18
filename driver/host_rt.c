#include <stdio.h>

extern void _cc_main(int *result);

int main(int argc, char *argv[]) {
    int result;
    _cc_main(&result);

    printf("%d\n", result);

    return 0;
}
