#!/bin/bash

TS=$1

rm magazines/*
rm pages/*
rm level1/*

./ts_to_es $2 < $TS | ./split_magazines magazines/ 
for x in magazines/*
do
	mn=`basename $x`
	./split_pages pages/$mn < $x
done

for x in pages/*
do
	bn=`basename $x`
	./page_to_level1 < $x > level1/$bn.l1
done
