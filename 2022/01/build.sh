#!/bin/sh

cc -Wall -Wextra -pedantic -g main1.c -o main1
cc -Wall -Wextra -pedantic -g main2.c -o main2

./main1 < input.txt
./main2 < input.txt

printf "\nCheck\nPart 1:\n"
echo `paste -s -d+ input.txt | sed 's/++/\n/g' | bc | sort -g | tail -1`
echo "Part 2:" 
echo `paste -s -d+ input.txt | sed 's/++/\n/g' | bc | sort -g | tail -3 | paste -s -d+ | bc`
