#!/bin/sh

CFLAGS="-std=c2x -ggdb -Wall -Wextra -Wpedantic"

cc $CFLAGS main.c -o main

./main < input.txt
