int foo(int x) {
    return x + 1;
}

int _cc_main(){
    int j = 0;
    for(int i = foo(j); i < foo(10); i = foo(i)) {
        j = foo(j);
    }
    return j;
}
