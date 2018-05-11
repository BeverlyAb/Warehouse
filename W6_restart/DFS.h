#ifndef DFS_H
#define DFS_H

#include "Layout.h"
#define ADJ_SIZE 4

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

  public:
    DFS() : Layout()
    {
      totalDist = 0;
      finalDest.x = 0; finalDest.y = 0;
    };
    
    DFS(unsigned int width, unsigned int height, string infile, position start, position end)
    :Layout(width, height, infile, start, end)
    {
      totalDist = 0;
      finalDest.x = 0; finalDest.y = 0;
    };
    
    void nextPos(position cur, position dest);
    bool isValidStop(position ref, position stop);
    bool isValid(position cur);
    void validNeighbors(position next);
    void printPath(position start, position end);
    void getPath();
    position getPos(unsigned int);
    position getFinalDest();

    //accessor
    int getTotalDist();
};
#endif