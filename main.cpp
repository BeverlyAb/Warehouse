#include "Parser.h"
#include "Mapper.h"
#include <ctime>
#include <iostream>

int main()
{
	int ans = 0;
	//clock_t startTime, endTime;
    Parser test = Parser();
    test.setUserParam();
	//startTime = clock();
    test.readFile(STOCK);
	printf( "Choose a number.\n Do you want to name orders (1), read all orders from a file (2), or check bounds (3)?\n");
	cin >> ans;
	
	if(ans > 0 && ans <=3){
		test.readFile(ans);
		if (!test.getAll() && !test.getBoundCheck()){
			test.getPath();
			test.opt();
			test.getPath();
		}
	}
	else {
		printf("Invalid choice. Ending Program\n");
		return 1;
	}

 
/*	
	endTime =  clock();
	int t = difftime(endTime, startTime);
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC); */
}
