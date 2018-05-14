#ifndef DFS_H
#define DFS_H

#include "Layout.h"
#include <stack>
#define ADJ_SIZE 4

#define MAX_ROW 105//183 //91*2 + 1
#define MAX_COL 105 //count 
#define INF 99999999

typedef struct moveSpace moveSpace;
struct moveSpace
{
  position loc;
  unsigned int hop;
  unsigned int weight;
};

class DFS : public Layout
{
  private:
    queue <position> neighbors;
	  map<position,moveSpace> path;
    int totalDist;
    position finalDest;
   
    map<position, int> dpRef;
    int dp[MAX_ROW][MAX_COL];

  public:
    DFS() : Layout()
    {
      totalDist = 0;
      finalDest.x = 0; finalDest.y = 0;
      for(int i = 0; i < MAX_ROW; i++){
        for(int j = 0; j < MAX_COL; j++){
          dp[i][j] = 0;
        }
      }
    };
    
    DFS(unsigned int width, unsigned int height, string infile, position start, position end)
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
    virtual void processSingleOrder(int);
    //accessor
    int getTotalDist();
    unsigned int pathOnly(position cur, position dest);
    void preProcess();
};
#endif