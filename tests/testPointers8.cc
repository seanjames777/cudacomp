int _cc_main() {
    int * * t = alloc(int*);
    int * t2 = alloc (int ) ;
    *t2 = 5;
    *t = t2;
    **t = **t + 15;
    return **t;
}
