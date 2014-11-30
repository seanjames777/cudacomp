struct cat {
    int meow;
};

int _cc_main() {
    struct cat * catty = alloc(struct cat);
    catty->meow = 1;
    return catty->meow;
}
