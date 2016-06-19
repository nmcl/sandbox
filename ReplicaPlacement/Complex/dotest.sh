#!/bin/sh
rm -f results.tmp results.sort Log Client_* out machineFile Machine_*
echo 'Working'
for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99
do
	echo $i
	#Simulation Random AC 5 800 80 0.99 $i 1 0 write >>results.tmp 2>>Log
	Simulation Simple AC 5 400 300 0.99 $i 1 0 write >>results.tmp 2>>Log
	mv Client_0_out client_$i
done

