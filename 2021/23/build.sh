#!/bin/sh

#CFLAGS="-std=c2x -g -march=native -Wall -Wextra -Wpedantic"
CFLAGS="-std=c2x -Ofast -march=native"

#cc $CFLAGS main1.c -o main1
cc $CFLAGS main2.c -o main2

#./main1 < sample.txt
#./main1 < input.txt
./main2 < sample.txt
#./main2 < input.txt