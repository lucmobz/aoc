#!/bin/sh

cc -Wall -Wextra -pedantic -g -DPART=1 -DROUND_COUNT=20 -DMONKEY_COUNT=8 main1.c -o main1
cc -Wall -Wextra -pedantic -g -DPART=2 -DROUND_COUNT=10000 -DMONKEY_COUNT=8 main1.c -o main2

./main1 < input.txt
./main2 < input.txt
