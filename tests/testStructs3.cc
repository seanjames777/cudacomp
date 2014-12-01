struct i32 {
    int bar;
};


int _cc_main() {
    struct i32 *baz = alloc(struct i32);
    (*baz).bar = 1;
    return (*baz).bar;
}
