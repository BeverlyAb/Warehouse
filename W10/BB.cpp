#include "BB.h"
BB::BB()
{
  size = 0;
}
BB::BB(queue<unsigned int> order, int size, int ** in)
{
  this->size = size;
  totalNodes = order;

  dp = new int * [size];
  for(int i = 0; i < size; i++){
    dp[i] = new int[size];
    for(int j = 0; j < size; j++)  
      dp[i][j] = in[i][j]; 
  }
}
void BB::updateTemp(map<int, int> & order, int ** dp, int ** temp)
{
  // printf("\ntemp\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      temp[i][j] = dp[i][j];
   //   printf("%i ", dp[i][j]);
    }
   // printf("\n");
  }
}

void BB::print(int ** arr, int index)
{
  printf("\nR%i\n",index);
  for(int i = 0; i < size; i++){
    for(int j = 0; j <size; j++)
      printf("%i ", arr[i][j]);
    printf("\n");
  }
} 

int ** BB::getDp()
{
  return dp;
}