#!/bin/sh

cc -Wall -Wextra -pedantic -g main1.c -o main1

./main1 < input.txt
