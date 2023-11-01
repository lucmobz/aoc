#!/bin/sh

cc -Wall -Wextra -pedantic -g main1.c -o main1
cc -Wall -Wextra -pedantic -g main2.c -o main2

./main1 < input.txt
./main2 < input.txt
