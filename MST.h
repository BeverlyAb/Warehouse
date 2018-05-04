#ifndef MST_H
#define MST_H
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Mapper.h"
#define ROW 100
#define MAX_COL 50
#define MAX_SIZE 2
#define STOCK 3
#define ORDER_FILE 2
using namespace std;

class MST
{
  private:
    //input parameters
    string inFile, outFile;
    string ans;
    unsigned int warehouseWidth;
   	unsigned int warehouseHeight;

    //data
    string ID, xCoord, yCoord;
  	Mapper grid;
  	position start,end;
  	queue<unsigned int> namedItems;
  	map<position, vector<unsigned int> > cluster;
	vector<vector<unsigned int> >orderFile;

  public:
    MST();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
	void getOrder(string file, int index);
    void getPath();

	//accessors
	int getWidth();
    int getHeight();

};
#endif
