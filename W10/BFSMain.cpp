#include "BFS.h"
#include <string>
#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
  if(argc != 9){
    printf("Error: Specify width, height, input file, start(x,y), end(x,y) -1 = USER input\n");
    return 1;
  }

  unsigned int width = atoi(argv[1]);
  unsigned int height = atoi(argv[2]);
  string infile = argv[3];

  unsigned int x = atoi(argv[4]);
  unsigned int y = atoi(argv[5]);
  position start = {x,y};

  x = atoi(argv[6]);
  y = atoi(argv[7]);
  position end = {x,y};
  int index = atoi(argv[8]);
  if(index == -1){
    string ans = "";
    printf("Order Index?\n");
    cin >> ans;
    index = atoi(ans.c_str());
  }

  BFS test = BFS(width, height, infile, start, end);
  test.readFile(STOCK, infile);

  // string act = "";
  // printf("Pick a number: name items (1) or read from list(2)?\n");
  // cin >> act;
  
  //should be after readFile
  clock_t startTime, endTime;
  
  test.makeRefDP();
  
  startTime = clock();
  test.preProcess();
  endTime =  clock();

  int t = difftime(endTime, startTime);
//	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

  // if(atoi(act.c_str()) == NAME_ITEM)
  //   test.readFile(NAME_ITEM, infile); //infile is dummy
  // else if(atoi(act.c_str()) == ORDER_FILE){
    // printf("Name order file\n");
    // cin >> infile;
    // test.readFile(ORDER_FILE, infile);
    test.getListItems("bigWarehouse.txt",0);
    for(int i = index; i < index + 1; i++){
    test.processSingleOrder(i);
      printf("INDEX %i\n", i);
      test.readWeight("weights.csv");
    }
 // }
  // else{
  //   printf("Invalid choice. Ending program\n");
  //   return 2;
  // }
  test.getPath();

  //2nd alg. Div
  test.alg2Div();
  test.getPath();
}

