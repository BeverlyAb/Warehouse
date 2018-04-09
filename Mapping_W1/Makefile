main: main.o Parser.o Mapper.o
	g++ main.o Parser.o Mapper.o -o main 

main.o: main.cpp
	g++ -c main.cpp

Parser.o: Parser.cpp
	g++ -c Parser.cpp

Mapper.o: Mapper.cpp
	g++ -c Mapper.cpp

clean:
	rm *.o

make:
	make clean
	make main
