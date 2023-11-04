#!/bin/sh

cc -Wall -Wextra -pedantic -g -DGRID_SIZE=99 main1.c -o main1

./main1 < input.txt
