#define ROW 5
#define COL 5
#define INF 999999

#include <map>
#include <queue>
#include <stdio.h>

using namespace std;
int main()
{
    map<int, int> order;//ID, index
    for(int i = 0; i < ROW; i++){
      order.insert(pair<int,int>(i,i));
    }
  queue<int> out;

  int dp[ROW][COL] = 
  { {INF, 12, 7, 8, 9}, 
    {12, INF, 5, 9, 3}, 
    {7, 5, INF, 10, 4},
    {8, 9, 10, INF, 6},
    {9, 3, 4, 6, INF}
  };
  printf("\nOriginal (R0)\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", dp[i][j]);
    printf("\n");
  }

  //init reduce 
  //row first
  int cost = 0;
  for(int i = 0; i < ROW; i++){
   
    int min = INF;
    for(int j = 0; j < COL; j++){
      if(dp[i][j] < min){
        min = dp[i][j];
       
      }
    }
    
    cost += min;
    
    for(int j = 0; j < COL; j++){
      dp[i][j] -= min;
    }
  }

  //this might be costly, because COLUMN miss
  for(int i = 0; i < COL; i++){
   
    int min = INF;
    for(int j = 0; j < ROW; j++){
      if(dp[j][i] < min)
        min = dp[j][i];
    }
    
    cost += min;
    
    for(int j = 0; j < ROW; j++){
      dp[j][i] -= min;
    }
  }

  printf("\nInit Reduce (R1)\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", dp[i][j]);
    printf("\n");
  }
  printf("LB %i\n", cost); // should be 26
  
    for(int j = 0; j < ROW; j++){
        dp[j][0] = INF;
    }
    
  int temp[ROW][COL];
  int tempCost[ROW] = {0};

  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      temp[i][j] = dp[i][j];
    }
  }
 
  int dest = 0;
  int index = 0;
  int src = 0;
  int oldCost = 0;
  int f = 2;
  
    map<int,int>::iterator outer = order.begin();
    outer = order.find(dest);
    order.erase(outer);
    int n = order.size();
    printf("size = %i", n);

    //iterate along COL and find lowest cost. Skipping COL 0. Continue down that branch
   n = order.size();
  while(/*oldCost != cost && */n > 0){
    map<int,int>::iterator outer = order.begin();
    for(; outer != order.end(); outer++){
      dest = outer->second;

      //null row of source, by default ROW 0 = start
      map<int,int>::iterator inner = order.begin();
      for(; inner != order.end(); inner++)
      {
        temp[src][inner->second] = INF;
      }
      //nullify COL of destination
      inner = order.begin();
      for(; inner != order.end(); inner++){
        temp[inner->second][dest] = INF;
      }
    
      int c = 0;
      //reduce along ROW
      inner = order.begin();
      for(; inner != order.end(); inner++){
      
        int min = INF;
        for(int j = 0; j < COL; j++){
          if(temp[inner->second][j] < min){
            min = temp[inner->second][j];
          }
        }
        if(min < INF -1000)//error correction
          c += min;
        else 
          min = 0;
        
        for(int j = 0; j < COL; j++){
          temp[inner->second][j] -= min;
        }
      }

      //this might be costly, because COLUMN miss
      inner = order.begin();
      for(; inner != order.end(); inner++){
      
        int min = INF;
        for(int j = 0; j < ROW; j++){
          if(temp[j][inner->second] < min)
            min = temp[j][inner->second];
        }
        
        if(min < INF-1000)
        {
          c += min;
        }
        else 
          min = 0;
        
        for(int j = 0; j < ROW; j++){
          temp[j][inner->second] -= min;
        }
      }
  
      tempCost[outer->second] = c + dp[src][dest] + cost;
     printf("\nR2 of %i\n", outer->second);
      for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++)
          printf("%i ", temp[i][j]);
        printf("\n");
      }
      printf("Cost for %i =  %i\n", outer->second, tempCost[outer->second]); 
  
      //reset temp
      for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
          temp[i][j] = dp[i][j];
        }
      }
    }

  int m = INF;
  for(int i = 0; i < COL; i++){
    if(m > tempCost[i] && i != src){
      m = tempCost[i];
      dest = i;
    }
  }
  //-------------------------------------------------
    //update dp as reduced matrix with lowest cost
    cost += tempCost[dest];
    out.push(dest);
    //null row of source, by default ROW 0 = start
    outer = order.begin();
    for(; outer != order.end(); outer++)
    {
      temp[src][outer->second] = INF;
    }
    //nullify COL of destination
    outer = order.begin();
    for(; outer != order.end(); outer++){
      temp[outer->second][dest] = INF;
    }

    int c = 0;
    //reduce along ROW
    outer = order.begin();
    for(; outer != order.end(); outer++){
    
      int min = INF;
      for(int j = 0; j < COL; j++){
        if(temp[outer->second][j] < min){
          min = temp[outer->second][j];
        }
      }
      if(min < INF -1000)//error correction
        c += min;
      else 
        min = 0;
      
      for(int j = 0; j < COL; j++){
        temp[outer->second][j] -= min;
      }
    }

    //this might be costly, because COLUMN miss
    outer = order.begin();
    for(; outer != order.end(); outer++){
    
      int min = INF;
      map<int,int>::iterator inner = order.begin();
      for(; inner != order.end(); inner++){
        if(temp[inner->second][outer->second] < min)
          min = temp[inner->second][outer->second];
      }
      
      if(min < INF-1000)
      {
        c += min;
      }
      else 
        min = 0;
      
      inner = order.begin();
      for(; inner != order.end(); inner++){
        temp[inner->second][outer->second] -= min;
      }
    }

    outer  = order.begin();
    map<int,int>::iterator inner = order.begin();
    for(; outer != order.end(); outer++){
        for(; inner != order.end(); inner++){
          dp[outer->second][inner->second] = temp[outer->second][inner->second];
        }
    }

    printf("\nR%i\n",f);
    for(int i = 0; i < ROW; i++){
      for(int j = 0; j < COL; j++)
        printf("%i ", dp[i][j]);
      printf("\n");
    }
  
    printf("Cost at R%i = %i\n",f, cost);

    src = dest;
    oldCost = cost;
    f++;

    outer = order.begin();
    outer = order.find(dest);
    order.erase(outer);
    n = order.size();
    printf("size = %i", n);
  }
 

  printf("\nOrder ");
  int m = out.size();
  for(int i = 0; i < m; i++){
    printf("%i->",out.front());
    out.pop();
  }
  printf("\n");
}
