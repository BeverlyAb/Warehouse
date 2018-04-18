#include "Parser.h"
#include "Mapper.h"
#include <iostream>

int main(int argc, char **argv)
{	
	if(argc != 8){
		printf("\tSpecify: filename, warehouse width, warehouse height\n\tstart-x, start-y, end-x, end-y\n");
		return -1;	
	}
	string file = argv[1];
	unsigned int width = atoi(argv[2]);
	unsigned int height = atoi(argv[3]);	
	int xStart = atoi(argv[4]);
	int yStart = atoi(argv[5]);
	int xEnd = atoi(argv[6]);
	int yEnd = atoi(argv[7]);	

    Parser test = Parser();
    test.setUserParam(file, width, height, xStart, yStart, xEnd, yEnd);;
    test.readFile(STOCK); 

	//printf( "Choose a number.\n Do you want to name orders (1) or read all orders from a file (2).\n");
	test.readFile(NAME_ITEM);
	test.getPath();
	test.opt();
	printf("----------------------------OPT----------------------\n");
	test.getPath();
}
