CC = g++-7 -fopenmp -lc++ #MAC
#CC = g++ -fopenmp #server 
CFLAGS = #-std=c++99
COPTFLAGS = -O3 -g -pg -Wall 
LDFLAGS =


LayoutMain: LayoutMain.o Layout.o 
	$(CC) $(COPTFLAGS) -o $@ $^

BFSMain:	BFSMain.o Layout.o BFS.o 
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm *.o

makeL:
	make LayoutMain

makeB:
	make BFSMain