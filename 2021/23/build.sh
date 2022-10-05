#!/bin/sh

#CFLAGS="-std=c2x -g -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -march=native"

cc $CFLAGS -Wno-unused-result main1.c -o main1
cc $CFLAGS -Wno-unused-result main2.c -o main2

./main1 < input.txt
./main2 < input.txt