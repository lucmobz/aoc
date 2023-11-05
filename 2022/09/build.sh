#!/bin/sh

cc -Wall -Wextra -pedantic -g -DPART=1 -DROPE_SIZE=2 main1.c -o main1
cc -Wall -Wextra -pedantic -g -DPART=2 -DROPE_SIZE=10 main1.c -o main2

./main1 < input.txt
./main2 < input.txt
