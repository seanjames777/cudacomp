typedef int secret;
typedef secret secret2;

secret add(secret x, secret2 y) {
    return x + y;
}

secret2 _cc_main() {
    return add(2, 3);
}
