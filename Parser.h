#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <algorithm>
#include "Mapper.h"

#define NAME_ITEM 1
#define ORDER_FILE 2
#define STOCK 3

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
	
  public:
    Parser();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
	void getNameItem();
	void getPath();
	void makeCluster(unsigned int);    
	void opt();

	//accessors
	int getWidth();
    int getHeight();
	
};
#endif
