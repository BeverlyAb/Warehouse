main: main.o Parser.o
	g++ main.o Parser.o -o main

main.o: main.cpp
	g++ -c main.cpp

Parser.o: Parser.cpp
	g++ -c Parser.cpp

clean:
	rm *.o

make:
	make clean
	make main
