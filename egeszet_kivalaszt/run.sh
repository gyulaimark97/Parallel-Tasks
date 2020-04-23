#!/bin/bash

for i in {1..15}
do
    ./app/egesz 1 par1
    ./app/egesz 2 par2
    ./app/egesz 4 par4
    ./app/egesz 8 par8
done

Rscript FIGURE.R
