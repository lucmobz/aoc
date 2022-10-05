#!/bin/sh

#CFLAGS="-std=c2x -g -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -march=native"

cc $CFLAGS main.c -o main

printf 'Part1: '
./main | head -1
printf 'Part2: '
./main | tail -1
