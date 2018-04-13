#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include "Mapper.h"

#define NAME_ORDER 1
#define ORDER_FILE 2
#define STOCK 3

using namespace std;

class Parser
{
  private:
    //input parameters
    string inFile;
    string outFile;
    string ans; 
    unsigned int warehouseWidth;
    unsigned int warehouseHeight;
    unsigned int mutator;
	unsigned int orderNum;

    //data
    string ID;
    string xCoord;
    string yCoord;
	Mapper grid;

  public:
    Parser();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
    int getWidth();
    int getHeight();
};
#endif
