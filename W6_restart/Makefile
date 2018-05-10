CC = g++
CFLAGS = -std=c++99
COPTFLAGS = -O3 -g -pg -Wall  #-fopenmp
LDFLAGS =


main: main.o Layout.o 
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm *.o

make:
	make clean
	make main
