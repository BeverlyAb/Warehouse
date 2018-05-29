#define ROW 5
#define COL 5
#define INF 999999

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>

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


int main()
{
  map<int, int> order;//ID, index
  for(int i = 0; i < ROW; i++){
    order.insert(pair<int,int>(i,i));
  }
  //init with null values
  int out[ROW];
  for(int i = 0; i < ROW; i++)
    out[i] = -1;

  int dp[ROW][COL] =   
  { {INF, 20, 30, 10, 11}, 
    {15, INF, 16, 4, 2}, 
    {3, 5, INF, 2, 4},
    {19, 6, 18, INF, 3},
    {16, 4, 7, 16, INF}
  };
 /*{{INF,	3,	5,	9,	11,	13,	15,	17,	19,	21},
{3,	INF,	2,	6,	8,	10,	12,	14,	16,	18},
{5,	2,	INF,	4,	6,	8,	10,	12,	14,	16},
{9,	6,	4,	INF,	2,	4,	6,	8,	10,	12},
{11,	8,	6,	2,	INF,	2,	4,	6,	8,	10},
{13,	10,	8,	4,	2,	INF,	2,	4,	6,	8},
{15,	12,	10,	6,	4,	2,	INF,	2,	4,	6},
{17,	14,	12,	8,	6,	4,	2,	INF,	2,	4},
{19,	16,	14,	10,	8,	6,	4,	2,	INF,	2},
{21,	18,	16,	12,	10,	8,	6,	4,	2,	INF}};*/


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

}