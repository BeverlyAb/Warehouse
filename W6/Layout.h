#ifndef LAYOUT_H
#define LAYOUT_H

#include <map>
#include <map>
#include <queue>
#include <list>

#define ADJ_SIZE 4
#define INF 999999

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

class Layout
{
  private:
    unsigned int width;
    unsigned int height;
    int totalDist;

    queue <position> neighbors;
	  map<position,moveSpace> path;

    vector<position> stock; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
	  position finalDest;
  
  public:
    Layout();
    Layout(unsigned int, unsigned int);
    void makeStock(unsigned int ID, unsigned int xCoord, unsigned int yCoord);
	  position getPos(unsigned int);
	  void nextPos(position cur, position dest);
    int nextPosBound(position cur, position dest);

	  bool isValidStop(position ref, position stop);
	  bool isValid(position cur);
    void validNeighbors(position next);

    void printPath(position start, position end);
	  position getFinalDest();
    int getTotalDist();
    map<position,moveSpace> getPath();
};
#endif