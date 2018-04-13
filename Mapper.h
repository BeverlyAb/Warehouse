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
	
	bool operator==(const position & right) const
	{	
		return (x == right.x && y == right.y);
	}  
	
	/*bool operator < (const position & right) const
    {
        return ((y < right.y) || (y == right.y &&  x < right.x));
    }*/
};

typedef struct moveSpace moveSpace;
struct moveSpace
{
	position loc;
    unsigned int hop;
}; 

using namespace std;
class Mapper
{
  private:
    unsigned int width;
    unsigned int height;
   
    queue <position> neighbors;
	map<position,moveSpace> path;
  
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
    void printPath(position start, position end);

//	bool lessVal(	const pair<position,unsigned int> & left,
//					const pair<position,unsigned int> & right);
	//position minVal(const map<position, unsigned int> &);  
    
    //friends
   // friend bool operator==(const position &, const position &);
	friend bool operator<(const position & , const position & );

};
#endif
