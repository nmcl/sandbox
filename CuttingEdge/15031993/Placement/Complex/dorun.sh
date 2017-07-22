#!/bin/sh
rm -f results.tmp Log
for i in 1 2 4 8 16 32 64 128 256
do
	echo 'Working'
	Simulation Simple AC 10 ${i}00 ${i}0 1.0 0.0 ${i}0 $i >>results.tmp 2>>Log
	echo >> results.tmp
done

