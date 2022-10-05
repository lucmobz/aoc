#!/bin/sh

#CFLAGS="-std=c2x -g -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -march=native"

cc $CFLAGS main.c -lm -o main

./main | head -1
./main | tail -1
