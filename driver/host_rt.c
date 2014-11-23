#include <stdio.h>

extern int _cc_main();

int rt_add(int x, int y) {
    return x + y;
}

int main(int argc, char *argv[]) {
    int result = _cc_main(&result);

    printf("%d\n", result);

    return 0;
}
