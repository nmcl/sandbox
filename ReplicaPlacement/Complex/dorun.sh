#!/bin/sh
rm -f results.tmp results.sort Log Client_* out machineFile Machine_*
echo 'Working'
Simulation Random AC 5 800 80 0.99 10.0 1 0 write >>results.tmp 2>>Log
#Simulation Simple AC 5 800 80 0.99 10.0 1 0 write >>results.tmp 2>>Log

