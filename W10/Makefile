#!/bin/bash
CC = g++-7 -fopenmp -lc++ #MAC
#CC = g++ -fopenmp #server 
CFLAGS = #-std=c++99
COPTFLAGS = -O3 -g -pg -Wall 
LDFLAGS =


LayoutMain: LayoutMain.o Layout.o 
	$(CC) $(COPTFLAGS) -o $@ $^

BFSMain:	BFSMain.o Layout.o BFS.o 
	$(CC) $(COPTFLAGS) -o $@ $^

bbmain:	bbmain.o Layout.o BFS.o
	$(CC) $(COPTFLAGS) -o $@ $^

final:	final.o Layout.o BB.o BFS.o
	$(CC) $(COPTFLAGS) -o $@ $^	

batch:	batch.o Layout.o BB.o BFS.o 
	$(CC) $(COPTFLAGS) -o $@ $^	

batchParser:	batchParser.o
	$(CC) $(COPTFLAGS) -o $@ $^	

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm *.o

makeL:
	make LayoutMain

makeB:
	make BFSMain

makeBB:
	make bbmain

makeBatch:
	make clean
	make
	make batch
	./batchRunner.sh

#change Index on Lines : 4 and 5
singleOrder:
	make clean
	make
	make batchParser 
	./batch 18 10 warehouse-grid.csv 10 11 35 20 5 > output.txt
	grep -n "INDEX 5" output.txt | cut -d : -f 1 > ind.txt
	./batchParser output.txt ind.txt > pathOnly.txt
	./PathMaker_clean/PathMaker.app/Contents/MacOS/PathMaker

#change Index on Lines : 4 
fromBatch:
	make clean
	make
	make batchParser 
	grep -n "INDEX 0" bigBatchProcess.txt | cut -d : -f 1 > ind.txt
	./batchParser bigBatchProcess.txt ind.txt > pathOnly.txt
	./PathMaker_clean/PathMaker.app/Contents/MacOS/PathMaker
