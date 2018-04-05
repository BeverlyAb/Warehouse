#ifndef MAPPER_H
#define MAPPER_H
#include <map>
#include <queue>
#include "Position.h"
#include "Product.h"

using namespace std;
class Mapper
{
  private:
    position start;
    position end;
    queue <position> path;
  public:
    Mapper();
    //look at malloc, map takes in ID and product
    map<unsigned int, position> makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
    //returns 2-D array 
    map<position, map<unsigned int,position> > makeGrid(position * grid, map<unsigned int,position> a, unsigned int width, unsigned int height);
   //use Dijkstra or BFS use queue
    position nextPos(position cur, product item);
    void printPath(); 
};
#endif