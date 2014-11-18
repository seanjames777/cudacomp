cat $1.cc | ../out/bin/cc > $1.host.bc
clang -o $1.o -c $1.host.bc
clang -o rt.o -c ../driver/rt.c
clang -o a.host.out $1.o rt.o

cat $1.cc | ../out/bin/cc --emit-device > $1.device.bc
cicc $1.device.bc

rm $1.host.bc
rm $1.o
rm rt.o
