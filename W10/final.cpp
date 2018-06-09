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

  test.readWeight("weights.csv");
  test.getPath();
  test.makeRefDP();
  test.preProcess();

  int ** arr = test.makeSubDP();
  queue<unsigned int> itemList = test.getItems();
  queue<unsigned int> d = test.getItems();
  //-------------------------------BB stuff
   queue<unsigned int> finalOrder;
  int startPos = 0;
  map<int, int> myorder;//ID, index
  myorder.insert(pair<int,int>(startPos, 0)); //include start!
  int size = itemList.size() + 1;           //include start!
  
  for(int i = 1; i < size; i++){
    int ID = itemList.front();
    itemList.pop();
    myorder.insert(pair<int,int>(i,i));
   printf("%i ", ID);
  }
  //-------------------------------------------------
 BB branch = BB(myorder, size, arr);
 branch.process();
 unsigned int intermediate[size];
 int IDs[size];
 for(int i = 1; i <size; i++){ //exclude start
    unsigned int ID = d.front();
  //   printf("FRONT %i\n", d.front());
    int itemIndx = test.getDPRef(ID);
    intermediate[i] = itemIndx;
    IDs[i] = ID;
  //   printf("intermediate %i , ind = %i\n", ID,intermediate[i]);
    d.pop();
  } 

  finalOrder = branch.mapBackToItems(intermediate, IDs);
  test.setOpt(finalOrder);
  test.getPath();
  return 0;
}