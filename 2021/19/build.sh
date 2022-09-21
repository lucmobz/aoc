#!/bin/sh

CFLAGS="-std=c2x -Ofast -Wall -Wextra -Wpedantic"
cc $CFLAGS main.c -o main

./main < sample.txt