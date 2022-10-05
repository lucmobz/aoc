#!/bin/sh

#CFLAGS="-std=c2x -ggdb3 -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -march=native"

cc $CFLAGS -Wno-unused-result main.c -o main

./main < input.txt