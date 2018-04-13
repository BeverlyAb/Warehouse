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
    void makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
	//returns 2-D array 
    position* makeGrid();
    void nextPos(position cur, unsigned int item);
    
	bool isValidStop(position ref, position stop);
	bool isValid(position cur);
    void validNeighbors(position next);
    
    void printPath(position start, position end);  
    
    //friends
    friend bool operator==(const position &, const position &);
	friend bool operator<(const position & , const position & );

};
#endif
