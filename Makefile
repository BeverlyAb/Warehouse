CC = g++
CFLAGS =
COPTFLAGS = -O3 -g -fopenmp
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
