#include "BB.h"
BB::BB()
{
  size = 0;
  dp = NULL;
  temp = NULL;
  initRed = NULL;
  out = NULL;
  index = 0;
  storeCost = NULL;
}
//destructor
BB::~BB()
{
  for(int i = 0; i < size; i++){
    free(dp[i]);
    free(temp[i]);
    free(initRed[i]);
  }
  free(dp);
  free(temp);
  free(initRed);
  free(out);
  free(storeCost);
}
BB::BB(map<int, int> order, int size, int ** in)
{
  BB();//assign to nulls

  this->size = size;
  this->order = order;
  index = 0;
  int buffer = 40;
  //populate dp, temp, and initRed  
  dp =  (int **)malloc(size * sizeof (int*));
  temp = (int **)malloc (size * sizeof (int*));
  initRed = (int **)malloc (size * sizeof (int*));

  out = (int *)malloc (size * sizeof (int));
  storeCost = (int *)malloc (size * sizeof (int));

  for(int i = 0; i < size; i++) { 
    dp[i] =   (int *)malloc(size * sizeof(int));
    temp[i] = (int *)malloc(size * sizeof(int));
    initRed[i] = (int *)malloc(size * sizeof(int));
  }


  for(int i = 0; i < size; i++){
    // dp[i] = new int[buffer];
    // temp[i] = new int[buffer];
    // initRed[i] = new int[buffer];

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
       //printf("cost1 = %i\n", cost);
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
      // printf("cost2 = %i\n", cost);
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
    temp[src][inner->second] = INF;
  }
//for debugging  
  // if(!reset){
  // printf("Before\n");
  // for(inner = order.begin(); inner != order.end(); inner++)
  //   printf("%i ", inner->first);
  // printf("\n");
  // }
  
  //must update values, not just resetting the new null
  if(!reset){ 
  //  inner = order.find(src);
    if(index < size -1){
      out[index++] = src;
     // printf("src = %i\n", src);
      order.erase(src);
      
      //for debugging
      // map<int, int>:: iterator itt = order.begin();
      // for(; itt != order.end(); itt++)
      //   printf("Order = ID %i = index %i ", itt->first, itt->second);
      // printf("\n");

    }
    else{
       out[index] = order.begin()->first;
     //  printf("src = %i\n", out[index]);
    }
  }

  //for debugging
  // if(!reset){
  // printf("After\n");
  //   for(inner = order.begin(); inner != order.end(); inner++)
  //     printf("%i ", inner->first);

  // printf("index %i", index);
  // printf("\n");
  // }
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
  //printf("dp[src][dest] = %i, src = %i, dest = %i\n", dp[src][dest],src,dest);
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
  // printf("\nOriginal");
  // print(dp, 0);
  int cost = 0;

  updateTemp();
  red(cost);
  
  // printf("\nInit Reduce");
  // print(temp,1);
  // printf("LB %i\n", cost); 

  updateOrig();
  
  int src = 0;
  int dest = 0;
  int tempCost = cost;

  nullSrc(src, false);
  updateTemp();
  // printf("\n Null src");
  // print(temp,1);

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
      
      // printf("\nNull dest for %i", outer->first);
      // print(temp, i);
      // printf("Cost %i\n", tempCost);
      
      storeCost[dest] = tempCost;

      //reset temp and null src
      tempCost = cost;
      updateTemp(); 
      nullSrc(src,true);
    }
    
    dest = findLeastCost(cost);
   // printf("final dest = %i\n", dest);
    totalCost(tempCost, src, dest);
    cost = storeCost[dest];
    //evaluate the matrix with least cost again and update original matrix
    nullSrc(src, true);
    nullDest(src, dest);
    red(cost);
    updateOrig();
    // print(dp, i);
    // printf("Cost %i\n", cost);

    src = dest;
    nullSrc(src, false);
    // print(temp,i);
    // printf("Cost %i\n", cost);

    map<int,int>::iterator it = order.begin();
    for(; it!= order.end(); it++){
//      printf("left%i over %i\n", i,it->second);
    }
  }
   for(int i = 0; i < size; i++){
    // printf("order %i\n", out[i]);
   }
}
queue<unsigned int> BB::mapBackToItems(unsigned int * intermediate, int * IDs)
{
  map<  unsigned int, int> intOrder;
 // int itemIndx = test.getDPRef(start);
  // printf("ind = %i\n", intermediate[0]);

  for(int i = 1; i < size; i++){ //exclude start
    intOrder.insert(pair< unsigned int, int>  (IDs[i], intermediate[i]));
  } 
  
  map< unsigned  int,int>::iterator it4 = intOrder.begin();
  for(; it4 != intOrder.end(); it4++){
  //printf("Indx %i , ID= %i\n", it4->first,it4->second);
  /*if(it4->second == -1)
    intOrder.erase(it4);
  */}

  sort(intermediate, size + intermediate);
  int table[size-1];
  //for(int i = 0; i < myROW; i++)
  //   printf("sort %i \n", intermediate[i]);


  for(int i = 0; i < size; i++){
    it4 = intOrder.begin();
    for(; it4 != intOrder.end(); it4++){
      if(it4->second == intermediate[i]){
        table[i] = it4->first;
        intOrder.erase(table[i]);
        finalOrder.push(table[i]);
        // printf("mappint %i %i\n", table[i], it4->second);
        break;
      }
    }
  } 

  //  for(int i = 0; i < myROW-1; i++)
 //      printf("table %i\n ", table[i]); 
// printf("here2\n");

  // for(int i = 0; i < size; i++){
  //   printf("finalOrderz %i out %i\n ", table[out[i]], out[i]);
  // } 
  return finalOrder;
}