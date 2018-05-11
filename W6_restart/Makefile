CC = g++
CFLAGS = -std=c++99
COPTFLAGS = -O3 -g -pg -Wall  #-fopenmp
LDFLAGS =


LayoutMain: LayoutMain.o Layout.o 
	$(CC) $(COPTFLAGS) -o $@ $^

DFSMain:	DFSMain.o Layout.o DFS.o 
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm *.o

makeL:
	make LayoutMain

makeD:
	make DFSMain