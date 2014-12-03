struct i32 {
    bool b;
};

int _cc_main() {
    struct i32 *b = alloc(struct i32);
    b->b = true;
    int x = 1; 
    if (b->b)
        return x;
    else
        return 0;
}
