#!/bin/sh

CFLAGS="-std=c2x -ggdb3 -Og -Wall -Wextra -Wpedantic"
cc $CFLAGS -Wno-sign-compare -Wno-unused-result -Wno-unused-variable main.c -o main

./main < sample.txt