#!/bin/bash

for x in *.c
do
	bn=`basename "$x" .c`
	gcc -o $bn "$x"
done
