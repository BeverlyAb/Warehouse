#include "BB.h"
BB::BB()
{
  size = 0;
  dp = 0;
  temp = 0;
  initRed = 0;
}
//destructor
BB::~BB()
{
  for(int i = 0; i < size; i++){
    delete[] dp[i];
    delete[] temp[i];
    delete[] initRed[i];
  }
  delete[] dp[;
  delete[] temp;
  delete[] initRed;
}
BB::BB(queue<unsigned int> order, int size, int ** in)
{
  this->size = size;
  totalNodes = order;


  //populate dp, temp, and initRed to be original matrix. Init purpose only
  dp = new int * [size];
  temp = new int * [size];
  initRed = new int * [size];

  for(int i = 0; i < size; i++){
    dp[i] = new int[size];
    temp[i] = new int[size];
    initRed[i] = new int[size];

    for(int j = 0; j < size; j++) { 
      dp[i][j] = in[i][j];
      temp[i][j] = in[i][j];
      initRed[i][j] = in[i][j]; 
    }
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

int ** BB::getArr(string name)
{
  if(name == "dp")
    return dp;
  else if(name == "temp")
    return temp;
  else if(name == "initRed")
    return initRed;
  else{
    printf("Invalid array name\n");
    return 0;
  }
}