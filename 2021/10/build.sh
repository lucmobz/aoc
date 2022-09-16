#!/bin/sh

CFLAGS="-ggdb -Wall -Wextra -Wpedantic"

cc -std=c2x -Wno-format $CFLAGS main.c -o main

./main < input.txt
