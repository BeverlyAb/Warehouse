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
  map<int, int> myorder;//ID, index
  myorder.insert(pair<int,int>(startPos, 0)); //include start!
  int size = itemList.size() + 1;           //include start!
  
  for(int i = 1; i < size; i++){
    int ID = itemList.front();
    itemList.pop();
    myorder.insert(pair<int,int>(i,i));
  //  printf("%i ", ID);
  }
  //-------------------------------------------------
  BB branch = BB(myorder, size, arr);
  branch.process();
//   myorder = branch.getOrder();
//   map<int, int>:: iterator itt = myorder.begin();
//   for(; itt !=  myorder.end(); itt++)
//      printf("Order = ID %i = index %i ", itt->first, itt->second);
//   printf("\n"); 

// //    //init with null values
// //   /*int out[size];
// //   for(int i = 0; i <size; i++)
// //     out[i] = -1;
// // */
//   clock_t startTime, endTime;
//   startTime = clock();
//   printf("\nOriginal");
//   branch.print(branch.getArr("dp"), 0);

//   int cost = 0;
//   int storeCost[size];

//   branch.updateTemp();
//   branch.red(cost);
//   printf("\nInit Reduce");
//   branch.print(branch.getArr("temp"),1);
//   printf("LB %i\n", cost); 

//   branch.updateOrig();

//   int src = 0;
//   int dest = 0;
//   int tempCost = cost;
//   //int index = 0;

//   branch.nullSrc(src, false);
//   branch.updateTemp();

//   printf("\n Null src");
//   branch.print(branch.getArr("temp"),1);
  
//   myorder = branch.getOrder();
//    map<int, int>::iterator outer =  myorder.begin();
//   //offset to 2 to reflect the n-th reductions

//   for(int i = 2; i < 2 + size; i++){
//     outer =  myorder.begin();
//     for(; outer !=  myorder.end(); outer++){
      
//       //reduce along dest
//       dest = outer->second;
//       branch.nullDest(src, dest);
//       branch.red(tempCost);
//       //printf("Cost1 %i\n", tempCost);
//       branch.totalCost(tempCost, src, dest);
      
//       //printf("Cost2 %i\n", tempCost);
      
//       printf("\nNull dest for %i", outer->first);
//       branch.print(branch.getArr("temp"), i);
//       printf("Cost %i\n", tempCost);
      
//       storeCost[dest] = tempCost;

//       //reset temp and null src
//       tempCost = cost;
//       branch.updateTemp(); 
//       branch.nullSrc(src, true);
//     }

//     dest = branch.findLeastCost(storeCost,  cost);
//     branch.totalCost(tempCost, src, dest);
//     cost = storeCost[dest];
//     //evaluate the matrix with least cost again and update original matrix
//     branch.nullSrc(src, true);
//     branch.nullDest(src, dest);
//     branch.red(cost);
//     branch.updateOrig();
//     branch.print(branch.getArr("dp"),i);
//     printf("Cost %i\n", cost);

//     src = dest;
//     branch.nullSrc(src,false);
//     myorder = branch.getOrder();
//     branch.print(branch.getArr("temp"),i);
//     printf("Cost %i\n", cost);

//     map<int,int>::iterator it =  branch.getOrder().begin();
//     for(; it!=  branch.getOrder().end(); it++){
//       printf("left%i over %i\n", i,it->second);
//     }
//   } 
//   for(int i = 0; i < size; i++)
//     printf("order %i\n", branch.get1DArr("out")[i]);

//   unsigned int intermediate[size];
//   map<  unsigned int, int> intOrder;
//  // int itemIndx = test.getDPRef(start);
//   // printf("ind = %i\n", intermediate[0]);

//   for(int i = 1; i <size; i++){ //exclude start
//     unsigned int ID = d.front();
//  //   printf("FRONT %i\n", d.front());
//     int itemIndx = test.getDPRef(ID);
//     intermediate[i] = itemIndx;
//     intOrder.insert(pair< unsigned int, int>  ( ID, itemIndx));
//  //   printf("intermediate %i , ind = %i\n", ID,intermediate[i]);
//     d.pop();
//   } 
  
//   map< unsigned  int,int>::iterator it4 = intOrder.begin();
//   for(; it4 != intOrder.end(); it4++){
//   //printf("Indx %i , ID= %i\n", it4->first,it4->second);
//   /*if(it4->second == -1)
//     intOrder.erase(it4);
//   */}

//   sort(intermediate, size + intermediate);
//   int table[size-1];
//   //for(int i = 0; i < myROW; i++)
//   //   printf("sort %i \n", intermediate[i]);


//   for(int i = 0; i < size; i++){
//     it4 = intOrder.begin();
//     for(; it4 != intOrder.end(); it4++){
//       if(it4->second == intermediate[i]){
//         table[i] = it4->first;
//         intOrder.erase(table[i]);
//         finalOrder.push(table[i]);
//         printf("mappint %i %i\n", table[i], it4->second);
//         break;
//       }
//     }
//   } 

//   //  for(int i = 0; i < myROW-1; i++)
//  //      printf("table %i\n ", table[i]); 
// // printf("here2\n");
//  /* for(int i = 0; i < myROW-1; i++){
//     finalOrder.push(table[out[i]]);

//     printf("finalOrderz %i out %i\n ", table[out[i]], out[i]);
//     //finalOrder.pop();
//   } */
//   //finalOrder.pop(); //remove start

//   test.setOpt(finalOrder);
//   test.getPath();
//     endTime =  clock();

//   int t = difftime(endTime, startTime);
// 	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);  

  return 0;
}