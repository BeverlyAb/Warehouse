#ifndef LAYOUT_H
#define LAYOUT_H

#define STOCK 0
#define NAME_ITEM 1
#define ORDER_FILE 2
#define ROW 100
#define COL 50

#include <string>
#include <map>
#include <queue>
#include <fstream>

using namespace std;

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

class Layout
{
  protected:
    string infile;
    unsigned int width, height;
    map<unsigned int, position> stock; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
  	
    position start,end;
  	queue<unsigned int> orgItems;
  	queue<unsigned int> optItems; 
    vector<vector<unsigned int> >orderFile;   

  public:
    Layout();
    Layout(unsigned int, unsigned int, string, position, position);
    void readFile(int, string);
    void stockSingleItem(unsigned int, unsigned int, unsigned int);
    void makeStock(string);
    void getNameItems();
    void getListItems(string, int);
    void processSingleOrder(int);
    void printStock();
};
#endif