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
	
	bool operator < (const position & right) const
    {
        return ((y < right.y) || (y == right.y &&  x < right.x));
    }
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
	map<position, unsigned int> cluster; 

	position finalDest;
  public:
    Mapper();
    Mapper(unsigned int w, unsigned int h);
    void makeStock(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
	position getPos(unsigned int);
	void makeCluster(unsigned int);    

	void nextPos(position cur, position dest);
    
	bool isValidStop(position ref, position stop);
	bool isValid(position cur);
    void validNeighbors(position next);
    
    void printPath(position start, position end); 
	position getFinalDest(); 
};
#endif
