#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Mapper.h"

#define NAME_ITEM 1
#define ORDER_FILE 2
#define STOCK 3
#define ROW 100
#define MAX_COL 50
#define MAX_SIZE 2

using namespace std;

class Parser
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
  	queue<unsigned int> optItems;
  	map<position, unsigned int> cluster;
	vector<vector<unsigned int> >orderFile;

  public:
    Parser();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
	void getNameItem();
	void getOrder(string file, int index);
	void getPath();
	void makeCluster(unsigned int);
	void opt();

	//accessors
	int getWidth();
    int getHeight();

};
#endif
