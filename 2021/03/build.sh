#!/bin/sh

#CFLAGS="-std=c2x -ggdb3 -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -Wno-unused-result -march=native"

cc $CFLAGS main.c -o main

./main < input.txt
