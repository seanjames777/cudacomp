typedef int tau;

int _cc_main() {
    tau * * t = alloc(tau*);
    tau * t2 = alloc (tau ) ;
    *t2 = 5;
    *t = t2;
    **t = **t + 15;
    return **t;
}
