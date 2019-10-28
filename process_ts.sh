#!/bin/bash

TS=$1

rm magazines/*
rm pages/*
rm cept/*

./ts_to_packet < $TS | ./split_magazines magazines/ 
for x in magazines/*
do
	mn=`basename $x`
	./split_pages pages/$mn < $x
done

for x in pages/*
do
	bn=`basename $x`
	./page_to_cept < $x > cept/$bn.cept
done
