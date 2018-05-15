#define ROW 5
#define COL 5
#define INF 999999

#include <map>
#include <queue>
#include <stdio.h>

using namespace std;
void OrigToTemp(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL])
{
  map<int, int>::iterator outer = order.begin();
  map<int, int>::iterator inner = order.begin();
  for(; outer != order.end(); outer++){
    for(; inner != order.end(); inner++){
      temp[outer->second][inner->second] = dp[outer->second][inner->second];
    }
  }
}

void tempToOrig(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL])
{
   map<int, int>::iterator outer = order.begin();
  map<int, int>::iterator inner = order.begin();
  for(; outer != order.end(); outer++){
    for(; inner != order.end(); inner++){
      dp[outer->second][inner->second] = temp[outer->second][inner->second];
    }
  }
}

void initRed(map<int, int> & order, int (& dp)[ROW][COL], int (&temp)[ROW][COL], int & cost, queue<int> &out, int & src, int & dest, int (&tempCost)[ROW])
{
  map<int, int>::iterator outer = order.begin();
  map<int, int>::iterator inner = order.begin();
  int c = 0;
  tempToOrig(order, dp, temp);
  
  //init reduce 
  //row first
  for(; outer != order.end(); outer++){

    int min = INF;
    for(; inner != order.end(); inner++){
      if(temp[outer->second][inner->second] < min){
        min = temp[outer->second][inner->second];
       
      }
    }
    if(min < INF -1000)//error correction
      c += min;
    else 
      min = 0;

    inner = order.begin();
    for(; inner != order.end(); inner++){
      temp[outer->second][inner->second] -= min;
    }
  }

  //this might be costly, because COLUMN miss
  outer = order.begin();
  for(; outer != order.end(); outer++){
   
    int min = INF;
    inner = order.begin();
    for(; inner != order.end(); inner++){
      if(temp[inner->second][outer->second] < min)
        min = temp[inner->second][outer->second];
    }
    
    if(min < INF -1000)//error correction
      c += min;
    else 
      min = 0;
    
    inner = order.begin();
    for(; inner != order.end(); inner++){
      temp[inner->second][outer->second] -= min;
    }
  }
  tempCost[src] = c + dp[src][dest] + cost;
}

void findLeastCost(int (& tempCost)[ROW], map<int, int> & order, int & dest)
{
  int m = INF;
  map<int, int>::iterator inner = order.begin();
    for(; inner != order.end(); inner++){
    if(m > tempCost[inner->second]){
      m = tempCost[inner->second];
      dest = inner->second;
    }
  }
}
void mainRed(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL], int & cost, queue<int> &out, int &src, int(&tempCost)[ROW])
{
  map<int, int>::iterator outer = order.begin();
  map<int, int>::iterator inner = order.begin();
  //copy over first
  for(; outer != order.end(); outer++){
    for(; inner != order.end(); inner++){
      temp[outer->second][inner->second] = dp[outer->second][inner->second];
    }
  }
  //null row of source
  inner = order.begin();
  for(; inner != order.end(); inner++){
    temp[src][inner->second] = INF;
  }

  outer = order.find(src);//remove visited node
  out.push(outer->second);
  order.erase(outer);
  int n = order.size();
  printf("size = %i", n);

  
  for(; outer != order.end(); outer++){
    inner = order.begin();
    for(; inner != order.end(); inner++){
      //null col of dest
      temp[inner->second][outer->second] = INF;
    }
    int dest = inner->second;
    initRed(order, dp, temp, cost, out, src, dest, tempCost);
    findLeastCost(tempCost, order, dest);
    OrigToTemp(order, dp, temp);
    src = dest;
  }
}

void finalRed(map<int, int> & order, int (& dp)[ROW][COL], int & cost, queue<int> &out, int &src, int(&tempCost)[ROW])
{

}

int main()
{
  map<int, int> order;//ID, index
    for(int i = 0; i < ROW; i++){
      order.insert(pair<int,int>(i,i));
    }
  queue<int> out;

  int dp[ROW][COL] = 
  { {INF, 20, 30, 10, 11}, 
    {15, INF, 16, 4, 2}, 
    {3, 5, INF, 2, 4},
    {19, 6, 18, INF, 3},
    {16, 4, 7, 16, INF}
  };
  printf("\nOriginal (R0)\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", dp[i][j]);
    printf("\n");
  }
  int cost = 0;
  int src = 0;
  
  int temp[ROW][COL];
  OrigToTemp(order, dp, temp);
  
  int tempCost[ROW];
  for(int i = 0; i < ROW; i++)
    tempCost[i] = 0;

  mainRed(order,dp,temp,cost,out,src,tempCost);
  printf("\nInit Reduce (R1)\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", dp[i][j]);
    printf("\n");
  }
  printf("LB %i\n", cost); // should be 26
  
  int n = order.size();
  int index = 2;
  for(int i = 0; i < n; i++)
    mainRed(order,dp,temp,cost,out,src,tempCost);
}
