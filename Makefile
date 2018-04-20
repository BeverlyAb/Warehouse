CC = g++
CFLAGS =   -std=c++99
COPTFLAGS = -O3 -g -pg  #-fopenmp
LDFLAGS =


main: main.o Parser.o Mapper.o
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm *.o

make:
	make clean
	make main
