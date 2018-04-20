#include "Parser.h"
#include "Mapper.h"
#include <iostream>

int main()
{
    Parser test = Parser();
    test.setUserParam();
    test.readFile(STOCK);

	//printf( "Choose a number.\n Do you want to name orders (1) or read all orders from a file (2).\n");
	test.readFile(NAME_ITEM);
	test.getPath();

  test.opt();
	printf("----------------------------OPT----------------------\n");
	test.getPath();
}
