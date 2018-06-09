#ifndef BB_H
#define BB_H
#define staticSize(x) x

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <queue>


using namespace std;

typedef struct Node Node;
struct Node
{
  int index;
  int cost;
  int depth;
  queue<int> visited;

  //sort by min. cost, then by deepest depth
  bool operator<(Node n2) const
  {
    if (cost < n2.cost)
      return 1;
    else if (cost == n2.cost)
      return depth > n2.depth;
    else
      return 0;
  }
};

class BB
{
  private:  
    queue<unsigned int> totalNodes;
    int size;
    //queue<unsigned int> visitedNodes;
    int **  dp;
    int  ** temp;
    int ** initRed;

  public:
    BB();
    BB(queue<unsigned int> order, int size, int ** in);
    void updateTemp(map<int, int> & order, int ** dp, int ** temp);
    int ** getDp();
  /*  void updateOrig(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL]);
    void nullSrc(map<int, int> & order, int (& dp)[ROW][COL], const int & src, int (&out)[ROW], int & index, bool reset);
    void nullDest(map<int, int> & order, int (& temp)[ROW][COL], const int & src, const int & dest, const int (&out) [ROW]);
    void red(map<int, int> & order, int (& temp)[ROW][COL], int & cost);
    void totalCost(int(&dp)[ROW][COL], int & cost, const int & src, const int & dest);
    int findLeastCost(int(&storeCost)[ROW], map<int, int> & order, int & cost); */
    void print(int ** arr, int index);
   /* void resetOrder(int(&out)[ROW] , map<int, int> & order, const map<int, int> & origOrder);
    void pointToArr(int ** in, int (&out)[ROW][COL]);  */
};
#endif