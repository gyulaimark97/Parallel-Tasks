#!/bin/bash
echo Fordítás és futtatás folyamatban, kérem várjon!
gcc auto.c -o auto -O1 -fopenmp 
for i in 1 2 3 4 5 
do
./auto O1.txt
done
echo Kész: 33%

gcc auto.c -o auto -O3 -fopenmp
for i in 1 2 3 4 5 
do
./auto O3.txt
done
echo Kész: 66%

gcc auto.c -o auto -fopenmp
for i in 1 2 3 4 5 
do
./auto no_auto.txt
done
echo Kész: 99%

Rscript figure.R
echo Kész: 100%
echo Megnézheti az eredményat a kep.eps fájlban