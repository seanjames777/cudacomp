struct cat {
    int cat;
    int dog;
    int doge;
};

int _cc_main() {
    struct cat * catty = alloc(struct cat);
    catty->cat = 5;
    catty->dog = 4;
    catty->doge = 4;
    return catty->cat + catty->dog + catty->doge;
}
