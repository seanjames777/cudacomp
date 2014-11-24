extern "C"
__device__
int rt_add(int x, int y) {
    return x + y;
}

extern "C"
__device__
int *rt_calloc(int count, int size) {
    int *buff = (int *)malloc(count * size);
    memset(buff, 0, count * size);
    return buff;
}
