#include "Parser.h"
#include "Mapper.h"
#include <iostream>

int main()
{
	int ans = 0;
    Parser test = Parser();
    test.setUserParam();
    test.readFile(STOCK);

	printf( "Choose a number.\n Do you want to name orders (1) or read all orders from a file (2).\n");
	cin >> ans;
	
	if(ans == NAME_ITEM || ans == ORDER_FILE){
		test.readFile(ans);
		test.getPath();
	} else {
		printf("Invalid choice. Ending Program\n");
		return 1;
	}

  test.opt();
	printf("----------------------------OPT----------------------\n");
	test.getPath();
}
