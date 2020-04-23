#!/bin/bash
for i in {1..15}
do
    ./app/Matrix 1 par1
    ./app/Matrix 2 par2
    ./app/Matrix 4 par4
    ./app/Matrix 8 par8
    echo $i. futtatás kész
done

Rscript FIGURE.R
