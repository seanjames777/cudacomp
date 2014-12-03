struct foo {
    int foo;
    int bar;
    int baz;
};

int _cc_main() {
    struct foo * foo_ptr = alloc(struct foo);
    foo_ptr->foo = 5;
    foo_ptr->bar = 4;
    foo_ptr->baz = 4;
    return foo_ptr->foo + foo_ptr->bar + foo_ptr->baz;
}
