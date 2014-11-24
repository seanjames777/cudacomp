extern "C"
__device__
int rt_add(int x, int y) {
    return x + y;
}

extern "C"
__device__
void *_rt_alloc_array(int elemSize, int length) {
    int buffSz = elemSize * length + 8;
    char *buff = (char *)malloc(buffSz);
    memset(buff, 0, buffSz);

    *((int *)&buff[0]) = length;
    *((int *)&buff[4]) = elemSize;

    return (void *)&buff[8];
}
