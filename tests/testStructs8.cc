struct cat {
    int cat;
};

struct tiger {
    struct cat c;
};

struct tigger {
    struct tiger *t;
};

struct quail {
    struct tigger t1;
    struct tigger *t2;
};

int _cc_main() {
    struct cat *catty = alloc(struct cat);
    catty->cat = 5;
    struct tiger *verycat = alloc(struct tiger);
    verycat->c.cat = catty->cat;
    struct tigger *notcat = alloc(struct tigger);
    notcat->t = verycat;
    struct quail *bird = alloc(struct quail);
    bird->t2 = notcat;
    bird->t1.t = notcat->t;
    catty->cat = 10;

    return bird->t1.t->c.cat + bird->t2->t->c.cat;
}
