#include "Layout.h"
#include "BFS.h"
#include "BB.h"
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 8){
    printf("Error: Specify width, height, input file, start(x,y), end(x,y)\n");
    return 1;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  string infile = argv[3];

  unsigned int x = atoi(argv[4]);
  unsigned int y = atoi(argv[5]);
  position start = {x,y};

  x = atoi(argv[6]);
  y = atoi(argv[7]);
  position end = {x,y};

  BFS test = BFS(width, height, infile, start, end);
  test.readFile(STOCK, infile);

  string act = "";
  printf("Pick a number: name items (1) or read from list(2)?\n");
  cin >> act;
  if(atoi(act.c_str()) == NAME_ITEM)
    test.readFile(NAME_ITEM, infile); //infile is dummy
  else if(atoi(act.c_str()) == ORDER_FILE){
   /* printf("Name order file\n");
    cin >> infile;
    test.readFile(ORDER_FILE, infile);*/
    test.readFile(ORDER_FILE, "warehouse-orders.csv");//change from hardcode later 
  } else{
    printf("Invalid choice. Ending program\n");
    return 2;
  }

  printf("Include weight? Yes (1) or No (0)\n");
  cin >> act;
  
  if(atoi(act.c_str()) == 1)
    test.readWeight("weights.csv");
  else if(atoi(act.c_str()) != 0 || atoi(act.c_str()) != 1){
    printf("Invalid choice. Ending program\n");
    return 2;
  }
  test.getPath();
  test.makeRefDP();
  test.preProcess();

  int ** arr = test.makeSubDP();
  queue<unsigned int> itemList = test.getItems();
  queue<unsigned int> d = test.getItems();
  //-------------------------------BB stuff
   queue<unsigned int> finalOrder;
  int startPos = 0;
  map<int, int> order;//ID, index
  order.insert(pair<int,int>(startPos, 0)); //include start!
  int size = itemList.size() + 1;           //include start!
  
  for(int i = 1; i < size; i++){
    int ID = itemList.front();
    itemList.pop();
    order.insert(pair<int,int>(i,i));
  //  printf("%i ", ID);
  }


  printf("%i\n", size);
  BB branch = BB(order, size, arr);

  map<int, int>:: iterator itt = order.begin();
  for(; itt != order.end(); itt++)
     printf("Order = ID %i = index %i ", itt->first, itt->second);
  printf("\n");

   //init with null values
  int out[size];
  for(int i = 0; i <size; i++)
    out[i] = -1;

  clock_t startTime, endTime;
  startTime = clock();
  printf("\nOriginal");
  branch.print(branch.getArr("dp"), 0);

  int cost = 0;
  int storeCost[size];

  branch.updateTemp();
  branch.red(cost);
  printf("\nInit Reduce");
  branch.print(branch.getArr("temp"),1);
  printf("LB %i\n", cost); 

  branch.updateOrig();

  int src = 0;
  int dest = 0;
  int tempCost = cost;
  int index = 0;

  branch.nullSrc(src, false);

  return 0;
}