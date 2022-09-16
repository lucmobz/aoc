#!/bin/sh

CFLAGS="-std=c2x -ggdb -Wall -Wextra -Wpedantic"

cc $CFLAGS -Wno-sign-compare main.c -o main

./main sample.txt
