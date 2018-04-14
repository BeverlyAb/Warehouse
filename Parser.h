#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
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
    unsigned int warehouseWidth, warehouseHeight;
   	unsigned int mutator;

    //data
    string ID, xCoord, yCoord;
	Mapper grid;
	position start,end;
	queue<unsigned int> namedItems;
	
  public:
    Parser();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
	void getNameItem();
	void getPath();

	//accessors
	int getWidth();
    int getHeight();
	
};
#endif
