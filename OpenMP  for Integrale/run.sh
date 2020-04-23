#!/bin/bash
for i in {1..15}
do
    ./app/Integral 1 par1
    ./app/Integral 2 par2
    ./app/Integral 4 par4
    ./app/Integral 8 par8
    echo $i. futtatás kész
done

Rscript FIGURE.R
