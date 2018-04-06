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
    unsigned int width;
    unsigned int height;
    queue <position> path;
  
    map<unsigned int, position> order; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
  public:
    Mapper();
    Mapper(unsigned int w, unsigned int h);
    //updates both maps (ID,product) and (Loc, Product);
    void makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
    //returns 2-D array 
    pair<position,bool> * Mapper::makeGrid(pair<position,bool> * grid, map<unsigned int,position> a);
   //use Dijkstra or BFS use queue
    position nextPos(position cur, product item, position * grid);
    bool isValidStop(product package, position stop);
     bool Mapper::isValidNeighbor(position cur, position next);
    void printPath(); 
};
#endif