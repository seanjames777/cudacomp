typedef int tau;

struct foo{
    tau baz;
};

typedef struct foo foo;

int main() {
    foo * bar = alloc(foo);
    (*bar).baz = 5;
    return (*bar).baz;
}
