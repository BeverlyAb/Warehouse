#!/bin/bash
# find missing indices from bigBatchProcess.txt and write to seg.txt
for ((k=0;k<2500;k+=1))
  do
    if grep -w "INDEX $k" bigBatchProcess.txt
    then
      echo "boo"
    else
      echo $k >> seg.txt
    fi
done

# re-run cases from seg.txt 
while IFS='' read -r line || [[ -n "$line" ]]; do
  ./batch 18 10 warehouse-grid.csv 0 0 0 20 $line >> bigBatchProcess.txt
done < "$1"