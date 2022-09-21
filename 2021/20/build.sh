#!/bin/sh

CFLAGS="-std=c2x -ggdb3 -Wall -Wextra -Wpedantic"
cc $CFLAGS main.c -o main

./main < input.txt