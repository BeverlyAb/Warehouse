#!/bin/bash
for ((k=0;k<100;k+=1))
  do
     ./batch 18 10 warehouse-grid.csv 0 0 0 20 $k >> bigBatchProcess.txt
done