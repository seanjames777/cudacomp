struct foo {
    int barone;
    int bartwo;
    int barthree;
    struct foo* f;
};

int _cc_main() {
    struct foo *baz = alloc(struct foo);
    (*baz).barone = 5;
    (*baz).bartwo = 10;
    (*baz).barthree = 15;
    (*baz).f = baz;
    return (*((*baz).f)).barone +
           (*((*baz).f)).bartwo +
           (*((*baz).f)).barthree;
}
