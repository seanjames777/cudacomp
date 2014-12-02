int _cc_main(){
    bool b = true;
    for( b ? true : false ; b ; b = b ? b : b) {
        bool c = !b;
        b = b ? c : c;
    }
    return 0;
}
