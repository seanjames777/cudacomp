extern "C"
__device__
void rt_add(int *result, int x, int y) {
    *result = x + y;
}
