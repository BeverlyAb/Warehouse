//#define ROW 5
//#define COL 5
//#define INF 999999

#include "Layout.h"
#include "BFS.h"
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <iostream>

using namespace std;
void updateTemp(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL])
{
 // printf("\ntemp\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      temp[i][j] = dp[i][j];
   //   printf("%i ", dp[i][j]);
    }
   // printf("\n");
  }
}

void updateOrig(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL])
{
 //  printf("\norig\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      dp[i][j] = temp[i][j];
   //   printf("%i ", dp[i][j]);
    }
   // printf("\n");
  }
}

void nullSrc(map<int, int> & order, int (& dp)[ROW][COL], const int & src, int (&out)[ROW], int & index, bool reset)
{ 
  map<int, int>::iterator inner = order.begin();
  //null row of source
  inner = order.begin();
  for(; inner != order.end(); inner++){
    dp[src][inner->second] = INF;
  }

  //must update values, not just resetting the new null
  if(!reset){ 
    inner = order.find(src);
    out[index++] = inner->second;
    order.erase(inner);
  }
}

void nullDest(map<int, int> & order, int (& temp)[ROW][COL], const int & src, const int & dest, const int (&out) [ROW])
{ 
  map<int, int>::iterator inner = order.begin();
  //null col of dest
  //still decide what dest is; that's why use temp
  for(; inner != order.end(); inner++){
    temp[inner->second][dest] = INF;
  }

  temp[dest][src] = INF;

  //null indirect backlinks
  for(int i = 0; i < ROW; i++){
    if(out[i] != -1)//is a valid, visited node
      temp[dest][out[i]] = INF;
    else
      break;
  }
}

void red(map<int, int> & order, int (& temp)[ROW][COL], int & cost)
{
 // map<int, int>::iterator outer = order.begin();
 // map<int, int>::iterator inner = order.begin();

  //reduce row first
  for(int i = 0; i < ROW; i++){

    int min = INF;
    for(int j = 0; j < COL; j++){
      if(temp[i][j] < min){
        min = temp[i][j];
       
      }
    }
    if(min < INF -10000){//error correction
      cost += min;
    }
    else 
      min = 0;

    for(int j = 0; j < COL; j++){
      temp[i][j] -= min;
    }
  }
 
  //this might be costly, because COLUMN miss
 // outer = order.begin();
  for(int i = 0; i < COL; i++){
   
    int min = INF;

    for(int j = 0; j < COL; j++){
      if(temp[j][i] < min)
        min = temp[j][i];
    }
    
    if(min < INF -10000){//error correction
      cost += min;
    }
    else 
      min = 0;
    
    for(int j = 0; j < COL; j++){
      temp[j][i] -= min;
    }
  }

}
void totalCost(int(&dp)[ROW][COL], int & cost, const int & src, const int & dest)
{
  cost += dp[src][dest];
 // printf("back link %i \n",dp[src][dest]);
}


//returns index of the node with least cost
int findLeastCost(int(&storeCost)[ROW], map<int, int> & order, int & cost)
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

void print(int(&arr)[ROW][COL], int index)
{
  printf("\nR%i\n",index);
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", arr[i][j]);
    printf("\n");
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
 // test.makeSubDP();
}

/*-------------------
map<int, int> order;//ID, index
  for(int i = 0; i < ROW; i++){
    order.insert(pair<int,int>(i,i));
  }
  //init with null values
  int out[ROW];
  for(int i = 0; i < ROW; i++)
    out[i] = -1;

  int dp[ROW][COL];


  clock_t startTime, endTime;
  startTime = clock();
  printf("\nOriginal");
  print(dp, 0);

  int cost = 0;
  int temp[ROW][COL];
  int storeCost[ROW];

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

  for(int i = 2; i < 1 + ROW; i++){
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
  for(int i = 0; i < ROW; i++)
    printf("order %i\n", out[i]);

    endTime =  clock();

  int t = difftime(endTime, startTime);
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
*/