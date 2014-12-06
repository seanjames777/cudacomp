struct tau {
    int x;
};

typedef struct tau tau;

int _cc_main() {
    tau *x = alloc(tau);
    x->x = 10;

    return x->x;
}
