cat $1.cc | ../out/bin/cc > $1.ll &&
clang -o $1.o -c $1.ll &&
clang -o rt.o -c ../driver/rt.c
clang -o a.out $1.o rt.o
rm $1.ll
rm $1.o
rm rt.o
