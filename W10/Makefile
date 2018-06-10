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

batch:	batch.o Layout.o BB.o BFS.o batchParser.o
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
