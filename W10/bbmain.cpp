#define myROW 6
#define myCOL 6
//myRow = orgItems.size() + 1
/* 
  myRow | ID Based on CSV | ID on main should be -1 Based on CSV
  6     | 4, 26, 27       | 3, 25, 26
  11    | 5, 43           | 4, 42
  21    | 10, 12, 69      | 9, 11, 68
*///#define INF 999999

#include "Layout.h"
#include "BFS.h"
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

void updateTemp(map<int, int> & order, int (& dp)[myROW][myCOL], int (& temp)[myROW][myCOL])
{
 // printf("\ntemp\n");
  for(int i = 0; i <myROW; i++){
    for(int j = 0; j <myCOL; j++){
      temp[i][j] = dp[i][j];
   //   printf("%i ", dp[i][j]);
    }
   // printf("\n");
  }
}

void updateOrig(map<int, int> & order, int (& dp)[myROW][myCOL], int (& temp)[myROW][myCOL])
{
 //  printf("\norig\n");
  for(int i = 0; i <myROW; i++){
    for(int j = 0; j <myCOL; j++){
      dp[i][j] = temp[i][j];
   //   printf("%i ", dp[i][j]);
    }
   // printf("\n");
  }
}

void nullSrc(map<int, int> & order, int (& dp)[myROW][myCOL], const int & src, int (&out)[myROW], int & index, bool reset)
{ 

  map<int, int>::iterator inner = order.begin();
  //nullmyROW of source
  inner = order.begin();

  for(; inner != order.end(); inner++){
    dp[src][inner->second] = INF;
  }
  
  if(!reset){
  printf("Before\n");
  for(inner = order.begin(); inner != order.end(); inner++)
    printf("%i ", inner->first);
  printf("\n");
  }
  
  //must update values, not just resetting the new null
  if(!reset){ 
  //  inner = order.find(src);
    if(index < myROW -1){
      out[index++] = src;
      printf("src = %i\n", src);
      order.erase(src);
    }
    else{
      out[index] = order.begin()->first;
       printf("src = %i\n", out[index]);
    }
  }

  if(!reset){
  printf("After\n");
    for(inner = order.begin(); inner != order.end(); inner++)
      printf("%i ", inner->first);

  printf("index %i", index);
  printf("\n");
  }
}

void nullDest(map<int, int> & order, int (& temp)[myROW][myCOL], const int & src, const int & dest, const int (&out) [myROW])
{ 
  map<int, int>::iterator inner = order.begin();
  //nullmyCOL of dest
  //still decide what dest is; that's why use temp
  for(; inner != order.end(); inner++){
    temp[inner->second][dest] = INF;
  }

  temp[dest][src] = INF;

  //null indirect backlinks
  for(int i = 0; i <myROW; i++){
    if(out[i] != -1)//is a valid, visited node
      temp[dest][out[i]] = INF;
    else
      break;
  }
}


void red(map<int, int> & order, int (& temp)[myROW][myCOL], int & cost)
{
 // map<int, int>::iterator outer = order.begin();
 // map<int, int>::iterator inner = order.begin();

  //reducemyROW first
  for(int i = 0; i <myROW; i++){

    int min = INF;
    for(int j = 0; j <myCOL; j++){
      if(temp[i][j] < min){
        min = temp[i][j];
       
      }
    }
    if(min < INF -10000){//error correction
      cost += min;
    }
    else 
      min = 0;

    for(int j = 0; j <myCOL; j++){
      temp[i][j] -= min;
    }
  }
 
  //this might be costly, becausemyCOLUMN miss
 // outer = order.begin();
  for(int i = 0; i <myCOL; i++){
   
    int min = INF;

    for(int j = 0; j <myCOL; j++){
      if(temp[j][i] < min)
        min = temp[j][i];
    }
    
    if(min < INF -10000){//error correction
      cost += min;
    }
    else 
      min = 0;
    
    for(int j = 0; j <myCOL; j++){
      temp[j][i] -= min;
    }
  }

}
void totalCost(int(&dp)[myROW][myCOL], int & cost, const int & src, const int & dest)
{
  cost += dp[src][dest];
 // printf("back link %i \n",dp[src][dest]);
}


//returns index of the node with least cost
int findLeastCost(int(&storeCost)[myROW], map<int, int> & order, int & cost)
{
  int m = INF;
  int tempDest = 0;
  //only check modified values
  map<int, int>::iterator inner = order.begin();
  for(; inner != order.end(); inner++){
    if(m > storeCost[inner->second]){
      m = storeCost[inner->second];
      tempDest = inner->second;
    }
  //  printf("min = %i, dest = %i\n", m, tempDest);
  }
  return tempDest;

}

void print(int(&arr)[myROW][myCOL], int index)
{
  printf("\nR%i\n",index);
  for(int i = 0; i <myROW; i++){
    for(int j = 0; j <myCOL; j++)
      printf("%i ", arr[i][j]);
    printf("\n");
  }
}


 void pointToArr(int ** in, int (&out)[myROW][myCOL])
  {
    for(int i = 0; i < myROW; i++){
      for(int j = 0; j < myCOL; j++)
        out[i][j] = in[i][j];
    }
  }

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

 /* #undef myROW
  #define myROW mySize
  #undef myCOL
  #define myCOL mySize*/

  int dp[myROW][myCOL];
  pointToArr(arr, dp);
 
  /*for(int i = 0; i < myROW; i++)
  {
    printf("d %i ", d.front());
    d.pop();
  }*/
  queue<unsigned int> finalOrder;
  //---------------------
  int startPos = 0;
  map<int, int> order;//ID, index

  order.insert(pair<int,int>(startPos, 0));
  
  for(int i = 1; i <myROW; i++){
    int ID = itemList.front();
    itemList.pop();
    order.insert(pair<int,int>(i,i));
  //  printf("%i ", ID);
  }
  map<int, int>:: iterator itt = order.begin();
  for(; itt != order.end(); itt++)
     printf("Order = ID %i = index %i ", itt->first, itt->second);
  printf("\n");

  //init with null values
  int out[myROW];
  for(int i = 0; i <myROW; i++)
    out[i] = -1;

  clock_t startTime, endTime;
  startTime = clock();
  printf("\nOriginal");
  print(dp, 0);

  int cost = 0;
  int temp[myROW][myCOL];
  int storeCost[myROW];

  updateTemp(order, dp, temp);
  red(order, temp, cost);
  
  printf("\nInit Reduce");
  print(temp,1);
  printf("LB %i\n", cost); 
  
  updateOrig(order, dp, temp);

  int src = 0;
  int dest = 0;
  int tempCost = cost;
  int index = 0;

  nullSrc(order, temp, src, out, index, false);
  updateTemp(order, dp, temp);
  
  printf("\n Null src");
  print(temp,1);

  map<int, int>::iterator outer = order.begin();
  //offset to 2 to reflect the n-th reductions

  for(int i = 2; i < 2 + myROW; i++){
    outer = order.begin();
    for(; outer != order.end(); outer++){
      
      //reduce along dest
      dest = outer->second;
      nullDest(order, temp, src, dest, out);
      red(order, temp, tempCost);
      //printf("Cost1 %i\n", tempCost);
      totalCost(dp, tempCost, src, dest);
      
      //printf("Cost2 %i\n", tempCost);
      
      printf("\nNull dest for %i", outer->first);
      print(temp, i);
      printf("Cost %i\n", tempCost);
      
      storeCost[dest] = tempCost;

      //reset temp and null src
      tempCost = cost;
      updateTemp(order, dp, temp); 
      nullSrc(order, temp, src, out, index, true);
    }
    
    dest = findLeastCost(storeCost, order, cost);
    totalCost(dp, tempCost, src, dest);
    cost = storeCost[dest];
    //evaluate the matrix with least cost again and update original matrix
    nullSrc(order, temp, src, out, index, true);
    nullDest(order, temp, src, dest, out);
    red(order, temp, cost);
    updateOrig(order, dp, temp);
    print(dp, i);
    printf("Cost %i\n", cost);

    src = dest;
    nullSrc(order, temp, src, out, index, false);
    print(temp,i);
    printf("Cost %i\n", cost);

    map<int,int>::iterator it = order.begin();
    for(; it!= order.end(); it++){
      printf("left%i over %i\n", i,it->second);
    }
  }
  for(int i = 0; i < myROW; i++)
    printf("order %i\n", out[i]);

  unsigned int intermediate[myROW];
  map<  unsigned int, int> intOrder;
 // int itemIndx = test.getDPRef(start);
  // printf("ind = %i\n", intermediate[0]);

  for(int i = 1; i < myROW; i++){ //exclude start
    unsigned int ID = d.front();
 //   printf("FRONT %i\n", d.front());
    int itemIndx = test.getDPRef(ID);
    intermediate[i] = itemIndx;
    intOrder.insert(pair< unsigned int, int>  ( ID, itemIndx));
 //   printf("intermediate %i , ind = %i\n", ID,intermediate[i]);
    d.pop();
  } 
  
  map< unsigned  int,int>::iterator it4 = intOrder.begin();
  for(; it4 != intOrder.end(); it4++){
  //printf("Indx %i , ID= %i\n", it4->first,it4->second);
  /*if(it4->second == -1)
    intOrder.erase(it4);
  */}

  sort(intermediate,myROW + intermediate);
  int table[myROW-1];
  //for(int i = 0; i < myROW; i++)
  //   printf("sort %i \n", intermediate[i]);


  for(int i = 0; i < myROW; i++){
    it4 = intOrder.begin();
    for(; it4 != intOrder.end(); it4++){
      if(it4->second == intermediate[i]){
        table[i] = it4->first;
        intOrder.erase(table[i]);
        finalOrder.push(table[i]);
        printf("mappint %i %i\n", table[i], it4->second);
        break;
      }
    }
  } 

  //  for(int i = 0; i < myROW-1; i++)
 //      printf("table %i\n ", table[i]); 
// printf("here2\n");
 /* for(int i = 0; i < myROW-1; i++){
    finalOrder.push(table[out[i]]);

    printf("finalOrderz %i out %i\n ", table[out[i]], out[i]);
    //finalOrder.pop();
  } */
  //finalOrder.pop(); //remove start

  test.setOpt(finalOrder);
  test.getPath();
    endTime =  clock();

  int t = difftime(endTime, startTime);
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);  
}
