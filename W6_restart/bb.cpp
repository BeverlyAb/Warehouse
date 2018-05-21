#include "BB.h"

BB::BB(map<int, int> totalNodes,  map<int, int> unvisitedNodes, int dpMat[ROW][COL])
{
  this->totalNodes = totalNodes;
  this->unvisitedNodes = unvisitedNodes;
  updateRight(order, dpMat, this-> dpMat);
}

void BB::updateRight(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL])
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

void BB::nullSrc(map<int, int> & order, int (& dp)[ROW][COL], const int & src, int (&out)[ROW], int & index, bool reset)
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

void BB::nullDest(map<int, int> & order, int (& temp)[ROW][COL], const int & src, const int & dest, const int (&out) [ROW])
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

void BB::red(map<int, int> & order, int (& temp)[ROW][COL], int & cost)
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

void BB::totalCost(int(&dp)[ROW][COL], int & cost, const int & src, const int & dest)
{
  cost += dp[src][dest];
 // printf("back link %i \n",dp[src][dest]);
}

//returns index of the node with least cost
int BB::findLeastCost(int(&storeCost)[ROW], map<int, int> & order, int & cost)
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

void BB::print(int(&arr)[ROW][COL], int index)
{
  printf("\nR%i\n",index);
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++)
      printf("%i ", arr[i][j]);
    printf("\n");
  }
}

void BB::resetOrder(int(&out)[ROW] , map<int, int> & order, const map<int, int> & origOrder)
{
  order = origOrder;
  for(int i = 0; i < ROW; i++){
    int tempID = out[i];
    map<int,int>::iterator it = order.find(tempID);
    order.erase(it);
  }
}

int * BB::downNextNode()
{
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
      updateRight(order, dp, temp); 
      nullSrc(order, temp, src, out, index, true);
    }
    
    dest = findLeastCost(storeCost, order, cost);
    totalCost(dp, tempCost, src, dest);
    cost = storeCost[dest];
    //evaluate the matrix with least cost again and update original matrix
    nullSrc(order, temp, src, out, index, true);
    nullDest(order, temp, src, dest, out);
    red(order, temp, cost);
    updateRight(order, temp, dp);
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
  return out;
}