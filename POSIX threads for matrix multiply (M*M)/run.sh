#!/bin/bash
for i in {1..15}
do
    ./app/Matrix  par
    echo $i. futtatás kész
done
Rscript FIGURE.R
