#!/bin/sh

#CFLAGS="-std=c2x -ggdb3 -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -Wno-unused-result -march=native"

cc $CFLAGS main1.c -o main1
cc $CFLAGS main2.c -o main2

./main1 < input.txt
./main2 < input.txt
