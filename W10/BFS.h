#ifndef BFS_H
#define BFS_H

#include "Layout.h"
#include <stack>
#include <omp.h>

#define ADJ_SIZE 4

#define MAX_ROW 93//183 //91*2 + 1
#define MAX_COL 93 //count based on shelf from t2.txt
#define INF 99999999

typedef struct moveSpace moveSpace;
struct moveSpace
{
  position loc;
  unsigned int hop;
  double weight;
  double effort;
};

class BFS : public Layout
{
  private:
    queue <position> neighbors;
	  map<position,moveSpace> path;
    map<unsigned int,double> weights;
    int totalDist;
    double totalEffort;
    position finalDest;
   
    map<position, int> dpRef; //position, Index to dp
    int dp2[MAX_ROW][MAX_COL]; //hops

  public:
    BFS() : Layout()
    {
      totalDist = 0;
      totalEffort = 0;
      finalDest.x = 0; finalDest.y = 0;
      for(int i = 0; i < MAX_ROW; i++){
        for(int j = 0; j < MAX_COL; j++){
          dp2[i][j] = 0;
        }
      }
    };
    
    BFS(unsigned int width, unsigned int height, string infile, position start, position end)
    :Layout(width, height, infile, start, end)
    {
      totalDist = 0;
      totalEffort = 0;
      finalDest.x = 0; finalDest.y = 0;

      for(int i = 0; i < MAX_ROW; i++){
        for(int j = 0; j < MAX_COL; j++){
          dp2[i][j] = 0;
        }
      }
    };
    
    void nextPos(position cur, position dest, unsigned int);
    bool isValidStop(position ref, position stop);
    bool isValid(position cur);
    void validNeighbors(position next);
    void printSinglePath(position start, position end);
    void getPath();
    position getPos(unsigned int);
    position getFinalDest();

    void makeRefDP();
    virtual void processSingleOrder(int);
    //accessor
    int getTotalDist();
    double getTotalEffort();
    void hopOnly(position cur, int newRowLeft);
    void hopOnlyNeighbors(position cur);
    void preProcess();
    void readWeight(string in);
    int ** makeSubDP();
    void setOpt(queue<unsigned int>);
    queue<unsigned int> getItems();
    int getItemSize();
    int getDPRef(position want);
    int getDPRef(unsigned int ID);
};
#endif