#include <stdio.h>

extern int _cc_main();

int main(int argc, char *argv[]) {
    int result = _cc_main(&result);

    printf("%d\n", result);

    return 0;
}
