#include "BB.h"
BB::BB()
{
  size = 0;
  dp = 0;
  temp = 0;
  initRed = 0;
  out = 0;
  index = 0;
  storeCost = 0;
}
//destructor
BB::~BB()
{
  for(int i = 0; i < size; i++){
    delete[] dp[i];
    delete[] temp[i];
    delete[] initRed[i];
  }
  delete[] dp;
  delete[] temp;
  delete[] initRed;
  delete[] out;
  delete[] storeCost;
}
BB::BB(map<int, int> order, int size, int ** in)
{
  this->size = size;
  this->order = order;
  index = 0;

  //populate dp, temp, and initRed to be original matrix. Init purpose only
  dp = new int * [size];
  temp = new int * [size];
  initRed = new int * [size];
  
  out = new int[size];
  storeCost = new int[size];

  for(int i = 0; i < size; i++){
    dp[i] = new int[size];
    temp[i] = new int[size];
    initRed[i] = new int[size];

    for(int j = 0; j < size; j++) { 
      dp[i][j] = in[i][j];
      temp[i][j] = 0;
      initRed[i][j] = 0; 
    }
  }

  resetOut();
}

void BB::resetOut()
{
  for(int i = 0; i < size; i++)
    out[i] = -1;
}
void BB::updateTemp()
{
  // printf("\ntemp\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      temp[i][j] = dp[i][j];
    //  printf("%i ", temp[i][j]);
    }
   // printf("\n");
  }
}
void BB::updateOrig()
{
 //  printf("\ndp\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      dp[i][j] = temp[i][j];
   //   printf("%i ", dp[i][j]);
    }
  //  printf("\n");
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

int * BB::get1DArr(string name)
{
  if(name == "out")
    return out;
  else{
    printf("Invalid array name\n");
    return 0;
  }
}
map<int,int> BB::getOrder()
{
  return order;
}

void BB::red(int & cost)
{
 // map<int, int>::iterator outer = order.begin();
 // map<int, int>::iterator inner = order.begin();

  //reduce row  first
  for(int i = 0; i < size; i++){

    int min = INF;
    for(int j = 0; j < size; j++){
      if(temp[i][j] < min){
        min = temp[i][j];
       
      }
    }
    if(min < INF -10000){//error correction
      cost += min;
       printf("cost1 = %i\n", cost);
    }
    else 
      min = 0;

    for(int j = 0; j < size; j++){
      temp[i][j] -= min;
    }
  }
 
  //this might be costly, becausemyCOLUMN miss
 // outer = order.begin();
  for(int i = 0; i < size; i++){
   
    int min = INF;

    for(int j = 0; j < size; j++){
      if(temp[j][i] < min)
        min = temp[j][i];
    }
    
    if(min < INF -10000){//error correction
      cost += min;
       printf("cost2 = %i\n", cost);
    }
    else 
      min = 0;
    
    for(int j = 0; j < size; j++){
      temp[j][i] -= min;
    }
  }
}

void BB::nullSrc(const int & src, bool reset)
{

  map<int, int>::iterator inner = order.begin();
  //null ROW of source
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
    if(index < size -1){
      out[index++] = src;
      printf("src = %i\n", src);
      order.erase(src);
      
      map<int, int>:: iterator itt = order.begin();
      for(; itt != order.end(); itt++)
        printf("Order = ID %i = index %i ", itt->first, itt->second);
      printf("\n");

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
void BB::print(int ** arr, int index)
{
  printf("\nR%i\n",index);
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++)
      printf("%i ", arr[i][j]);
    printf("\n");
  }
} 
void BB::nullDest(const int & src, const int & dest)
{
  map<int, int>::iterator inner = order.begin();
  //nullmyCOL of dest
  //still decide what dest is; that's why use temp
  for(; inner != order.end(); inner++){
    temp[inner->second][dest] = INF;
  }

  temp[dest][src] = INF;

  //null indirect backlinks
  for(int i = 0; i < size; i++){
    if(out[i] != -1)//is a valid, visited node
      temp[dest][out[i]] = INF;
    else
      break;
  }
}
void BB::totalCost(int & cost, const int & src, const int & dest)
{
  cost += dp[src][dest];
}
int BB::findLeastCost(int & cost)
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

void BB::process()
{
  printf("\nOriginal");
  print(dp, 0);
  int cost = 0;
  int storeCost[size];

  updateTemp();
  red(cost);
  
  printf("\nInit Reduce");
  print(temp,1);
  printf("LB %i\n", cost); 

  updateOrig();

  int src = 0;
  int dest = 0;
  int tempCost = cost;

  nullSrc(src, false);
  updateTemp();
  printf("\n Null src");
  print(temp,1);

  map<int, int>::iterator outer = order.begin();

  for(int i = 2; i < 2 + size; i++){
    outer = order.begin();
    for(; outer != order.end(); outer++){
      
      //reduce along dest
      dest = outer->second;
      nullDest(src, dest);
      red(tempCost);
      //printf("Cost1 %i\n", tempCost);
      totalCost(tempCost, src, dest);
      
      //printf("Cost2 %i\n", tempCost);
      
      printf("\nNull dest for %i", outer->first);
      print(temp, i);
      printf("Cost %i\n", tempCost);
      
      storeCost[dest] = tempCost;

      //reset temp and null src
      tempCost = cost;
      updateTemp(); 
      nullSrc( src,true);
    }
    
    dest = findLeastCost(cost);
    totalCost(tempCost, src, dest);
    cost = storeCost[dest];
    //evaluate the matrix with least cost again and update original matrix
    nullSrc(src, true);
    nullDest(src, dest);
    red(cost);
    updateOrig();
    print(dp, i);
    printf("Cost %i\n", cost);

    src = dest;
    nullSrc(src, false);
    print(temp,i);
    printf("Cost %i\n", cost);

    map<int,int>::iterator it = order.begin();
    for(; it!= order.end(); it++){
      printf("left%i over %i\n", i,it->second);
    }
  }
   for(int i = 0; i < size; i++)
    printf("order %i\n", out[i]);
}