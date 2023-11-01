#!/bin/sh

cc -Wall -Wextra -pedantic -g -DPART=1 -DWINDOW_SIZE=4 main1.c -o main1
cc -Wall -Wextra -pedantic -g -DPART=2 -DWINDOW_SIZE=14 main1.c -o main2

./main1 < input.txt
./main2 < input.txt
