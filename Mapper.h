#ifndef MAPPER_H
#define MAPPER_H
#include <map>
#include <queue> 
#include "Position.h"
#include "Product.h"
#define ADJ_SIZE 4

using namespace std;
class Mapper
{
  private:
    unsigned int width;
    unsigned int height;
    queue <position> path;
    queue <position> neighbors;
  
    map<unsigned int, position> order; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
  public:
    Mapper();
    Mapper(unsigned int w, unsigned int h);
    //updates both maps (ID,product) and (Loc, Product);
    void makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
    //returns 2-D array 
    position* makeGrid(position* grid);
   //use Dijkstra or BFS use queue
    void nextPos(position cur, product item, position * grid);
    bool isValidStop(product package, position stop);
    bool isValidNeighbor(position next);
    void validNeighbors(position cur);
    int shortest(int label[], bool visited[], position * grid, position cur);
    void printPath(); 
    
    //friends
    friend bool operator==(const position &, const position &);
};
#endif