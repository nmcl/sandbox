#!/bin/sh
rm -f Results results.tmp Log
for i in 1 2 4 8 16 32 64 128 256
do
    echo $i
    Simulation Simple AC 10 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
    Simulation Random AC 1 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
    Simulation Random AC 2 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
    Simulation Random AC 3 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
    Simulation Random AC 4 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
    Simulation Random AC 5 ${i}00 ${i}0 .99 $i >>results.tmp 2>>Log
done
echo "Policy	Protocol	Avail(g)	#ofReplicas	Avail(m)" > Results
echo  >> Results
sort results.tmp >> Results
rm results.tmp
