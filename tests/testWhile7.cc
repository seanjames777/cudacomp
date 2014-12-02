int _cc_main() {
    bool b = true;
    while ( b ? true : false ) {
        b = b ? false : true;
    }
    return 0;
}
