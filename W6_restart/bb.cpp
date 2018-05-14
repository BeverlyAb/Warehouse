#define ROW 5
#define COL 5
#define INF 999999

#include <stdio.h>
int main()
{
  int order[ROW];

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
  //iterate along COL and find lowest cost. Skipping COL 0. Continue down that branch
  for(int l = 1; l < ROW; l++){
    dest = l;
    order[index++] = dest;

    //null row of source, by default ROW 0 = start
   
    for(int i = 0; i < COL; i++)
    {
      temp[src][i] = INF;
    }
    //nullify COL of destination
    for(int j = 0; j < ROW; j++){
      temp[j][dest] = INF;
    }
  
    int c = 0;
    //reduce along ROW
    for(int i = 0; i < ROW; i++){
    
      int min = INF;
      for(int j = 0; j < COL; j++){
        if(temp[i][j] < min){
          min = temp[i][j];
        }
      }
      if(min < INF -1000)//error correction
        c += min;
      else 
        min = 0;
      
      for(int j = 0; j < COL; j++){
        temp[i][j] -= min;
      }
    }

    //this might be costly, because COLUMN miss
    for(int i = 0; i < COL; i++){
    
      int min = INF;
      for(int j = 0; j < ROW; j++){
        if(temp[j][i] < min)
          min = temp[j][i];
      }
      
      if(min < INF-1000)
      {
        c += min;
      }
      else 
        min = 0;
      
      for(int j = 0; j < ROW; j++){
        temp[j][i] -= min;
      }
    }
    tempCost[l] = c + dp[src][dest] + cost;
   /* printf("\nR2 of %i\n", l);
    for(int i = 0; i < ROW; i++){
      for(int j = 0; j < COL; j++)
        printf("%i ", temp[i][j]);
      printf("\n");
    }
    printf("Cost for %i =  %i\n", l, tempCost[l]); 
*/
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
 
  //null row of source, by default ROW 0 = start
  
  for(int i = 0; i < COL; i++)
  {
    temp[src][i] = INF;
  }
  //nullify COL of destination
  for(int j = 0; j < ROW; j++){
    temp[j][dest] = INF;
  }

  int c = 0;
  //reduce along ROW
  for(int i = 0; i < ROW; i++){
  
    int min = INF;
    for(int j = 0; j < COL; j++){
      if(temp[i][j] < min){
        min = temp[i][j];
      }
    }
    if(min < INF -1000)//error correction
      c += min;
    else 
      min = 0;
    
    for(int j = 0; j < COL; j++){
      temp[i][j] -= min;
    }
  }

  //this might be costly, because COLUMN miss
  for(int i = 0; i < COL; i++){
  
    int min = INF;
    for(int j = 0; j < ROW; j++){
      if(temp[j][i] < min)
        min = temp[j][i];
    }
    
    if(min < INF-1000)
    {
      c += min;
    }
    else 
      min = 0;
    
    for(int j = 0; j < ROW; j++){
      temp[j][i] -= min;
    }
  }


  for(int i = 0; i < ROW; i++){
      for(int j = 0; j < COL; j++){
        dp[i][j] = temp[i][j];
      }
  }

  printf("\nR2\n");
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", dp[i][j]);
    printf("\n");
  }
  printf("Cost at R2 = %i\n", cost);
}