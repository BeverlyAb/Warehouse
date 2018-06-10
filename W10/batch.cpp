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
  if(argc != 9){
    printf("Error: Specify width, height, input file,  -1 = USER INPUT\n");
    return 1;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  string infile = argv[3];

  unsigned int x = atoi(argv[4]);
  unsigned int y = atoi(argv[5]);
  //if index == -1, talk to user, otherwise run batch
  int index = atoi(argv[8]);
  if(index == -1){
    string ans = "";
    printf("Order Index?\n");
    cin >> ans;
    index = atoi(ans.c_str());
  }

  position start = {x,y};

  x = atoi(argv[6]);
  y = atoi(argv[7]);
  position end = {x,y};

  BFS test = BFS(width, height, infile, start, end);
  test.readFile(STOCK, infile);

  test.getListItems("bigWarehouse.txt",0);//change from hardcode later 
  int once = 0;
  for(int i = index; i < index + 1; i++){
    test.processSingleOrder(i);
      printf("INDEX %i\n", i);
   test.readWeight("weights.csv");
    test.getPath();
    if(once == 0){
      test.makeRefDP();
      test.preProcess();
      once++;
    }
  
    int ** arr = test.makeSubDP();
    queue<unsigned int> itemList = test.getItems();
    queue<unsigned int> d = test.getItems();
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
  }

  return 0;
}