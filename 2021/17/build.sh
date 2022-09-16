#!/bin/sh

CFLAGS="-ggdb -Wall -Wextra -Wpedantic"

cc -std=c2x $CFLAGS main.c -lm -o main

./main < sample.txt
