#ifndef BFS_H
#define BFS_H

#include "Layout.h"
#include <stack>
#include <omp.h>

#define ADJ_SIZE 4

#define MAX_ROW 103//183 //91*2 + 1
#define MAX_COL 103 //count based on shelf from t2.txt
#define INF 99999999

typedef struct moveSpace moveSpace;
struct moveSpace
{
  position loc;
  unsigned int hop;
  unsigned int weight;
};

class BFS : public Layout
{
  private:
    queue <position> neighbors;
	  map<position,moveSpace> path;
    map<int, int> weights;
    int totalDist;
    position finalDest;
   
    map<position, int> dpRef; //position, ID
    int dp[MAX_ROW][MAX_COL]; //hops, where indices are ID

  public:
    BFS() : Layout()
    {
      totalDist = 0;
      finalDest.x = 0; finalDest.y = 0;
      for(int i = 0; i < MAX_ROW; i++){
        for(int j = 0; j < MAX_COL; j++){
          dp[i][j] = 0;
        }
      }
    };
    
    BFS(unsigned int width, unsigned int height, string infile, position start, position end)
    :Layout(width, height, infile, start, end)
    {
      totalDist = 0;
      finalDest.x = 0; finalDest.y = 0;

      for(int i = 0; i < MAX_ROW; i++){
        for(int j = 0; j < MAX_COL; j++){
          dp[i][j] = 0;
        }
      }
    };
    
    void nextPos(position cur, position dest);
    bool isValidStop(position ref, position stop);
    bool isValid(position cur);
    void validNeighbors(position next);
    void printSinglePath(position start, position end);
    void getPath();
    position getPos(unsigned int);
    position getFinalDest();

   // virtual void getListItems(string in, int index);
    //virtual readFile();
    void makeRefDP();
    virtual void processSingleOrder(int);
    //accessor
    int getTotalDist();
    void hopOnly(position cur, int newRowLeft);
    void hopOnlyNeighbors(position cur);
    void preProcess();
    void readWeight(string in);
};
#endif