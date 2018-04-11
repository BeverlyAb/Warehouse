#ifndef MAPPER_H
#define MAPPER_H

#include <map>
#include <queue> 
#include <list>
#include <cstdlib>
#include <stdio.h>

//#include "Position.h"
//#include "Product.h"

#define ADJ_SIZE 4
#define INF 99999999999

typedef struct position position;
struct position
{
    unsigned int x;
    unsigned int y;
  
	bool operator < (const position & right) const
    {
        return ((y < right.y) || (y == right.y &&  x < right.x));
    }
};

typedef struct product product;
struct product
{
    unsigned int ID;
    position loc;
}; 

using namespace std;
class Mapper
{
  private:
    unsigned int width;
    unsigned int height;
    queue <position> path;
    queue <position> neighbors;
  
    map<unsigned int, position> stock; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
  public:
    Mapper();
    Mapper(unsigned int w, unsigned int h);
    //updates both maps (ID,product) and (Loc, Product);
    void makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
    //returns 2-D array 
    position* makeGrid();
   //use Dijkstra or BFS use queue
    void nextPos(position cur, unsigned int item);
    
	bool isValidStop(position ref, position stop);
	bool isValid(position cur);
    void validNeighbors(position next);
    
	int shortest(int label[], bool visited[], position * grid, position cur);
    void printPath(); 
    
    //friends
    friend bool operator==(const position &, const position &);
	//friend bool operator<(const position & , const position & );
};
#endif
