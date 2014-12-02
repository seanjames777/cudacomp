int _cc_main(){
    int j = 0;
    for(int i = 0; i < 10; i = i + 1) {
        for (int k = 0; k < 10; k = k + 1) {
            j = j + i + k;
        }
    }
    return j;
}
