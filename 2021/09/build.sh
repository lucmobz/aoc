#!/bin/sh

CFLAGS="-ggdb -Wall -Wextra -Wpedantic"

cc -std=c2x $CFLAGS main.c -o main

./main < input.txt
