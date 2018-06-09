#ifndef BB_H
#define BB_H
#define INF 999999

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <map>
#include <queue>
#include <string>


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
    map<int, int> order;
    int size;
    //queue<unsigned int> visitedNodes;
    int **  dp;
    int  ** temp;
    int ** initRed;
    int * out;

  public:
    BB();
    ~BB();
    BB(map<int, int> order, int size, int ** in);
    void resetOut();
    void updateTemp();
    void updateOrig();
    int index;
    int ** getArr(string name);
    int * get1DArr(string name);
    //reduces temp only!
    void red(int & cost);
    void nullSrc(const int & src, bool reset);
   void nullDest(const int & src, const int & dest);
 void totalCost(int & cost, const int & src, const int & dest);
    int findLeastCost(int * storeCost,int & cost); 
    void print(int ** arr, int index);
   /* void resetOrder(int(&out)[ROW] , map<int, int> & order, const map<int, int> & origOrder);
    void pointToArr(int ** in, int (&out)[ROW][COL]);  */
};
#endif