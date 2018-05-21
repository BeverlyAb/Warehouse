#ifndef BB_H
#define BB_H
#define ROW 10
#define COL 10
#define INF 999999

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <queue>


using namespace std;

typedef struct Node Node;
struct Node
{
  int parent;
  int index;
  int cost;
  int depth;
  int out[ROW];//visited
};

//sort by min. cost, then by deepest depth
class nodeComparator
{
  public:
  int operator() (const Node & n1, const Node & n2)
  {
    if (n1.cost < n2.cost)
      return 1;
    else if (n1.cost == n2.cost)
      return n1.depth > n2.depth;
    else
      return 0;
  }
};

class BB
{
  private:  
    map<int, int> totalNodes;
    map<int, int> unvisitedNodes;//unvisited
    int dpMat[ROW][COL];
    int tempMat[ROW][COL];
    int initRed[ROW][COL];

  public:
    BB(map<int, int> totalNodes,  map<int, int> unvisitedNodes, int dpMat[ROW][COL]);
    void updateRight(map<int, int> & order, int (& dp)[ROW][COL], int (& temp)[ROW][COL]);
    void nullSrc(map<int, int> & order, int (& dp)[ROW][COL], const int & src, int (&out)[ROW], int & index, bool reset);
    void nullDest(map<int, int> & order, int (& temp)[ROW][COL], const int & src, const int & dest, const int (&out) [ROW]);
    void red(map<int, int> & order, int (& temp)[ROW][COL], int & cost);
    void totalCost(int(&dp)[ROW][COL], int & cost, const int & src, const int & dest);
    int findLeastCost(int(&storeCost)[ROW], map<int, int> & order, int & cost);
    void print(int(&arr)[ROW][COL], int index);
    void resetOrder(int(&out)[ROW] , map<int, int> & order, const map<int, int> & origOrder);
    //fix later
    int * downNextNode();
};
#endif