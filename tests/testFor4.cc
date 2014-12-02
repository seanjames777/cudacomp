int _cc_main(){
    int sum = 0;
    for(int i = 0; i < 3; i = i + 1) {
        for(int j = 0; j < 3; j = j + 1) {
            for(int k = 1; k < 3; k = k + 1) {
                sum = sum + (i + j + k);
            }
        }
    }
    return sum;
}
