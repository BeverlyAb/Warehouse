#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Mapper.h"

#define NAME_ITEM 1
#define ORDER_FILE 2
#define BOUND 3
#define STOCK 4
#define ROW 100
#define MAX_COL 50
#define MAX_SIZE 2

using namespace std;
#define MAX 150

class Parser
{
  private:
    //input parameters
    string inFile, outFile;
    string ans;
    unsigned int warehouseWidth;
   	unsigned int warehouseHeight;

	int dp[MAX][MAX];
    int dpIndx;
    map<int, position> dpMap;
    //data
    string ID, xCoord, yCoord;
  	Mapper grid;
  	position start,end;
  	queue<unsigned int> namedItems;
  	queue<unsigned int> optItems;
  	map<position, vector<unsigned int> > cluster;
	vector<vector<unsigned int> >orderFile;

	bool all;
	bool boundCheck;

  public:
    Parser();
    void setUserParam();
    void readFile(int fileType);
	void readOrder();
	void getNameItem();
	void getOrder(string file, int index, int fileType);
	void getPath();
	void makeCluster(unsigned int);
	void opt();
	void branchBound();
	int reduce(int, int, bool);

	//accessors
	int getWidth();
    int getHeight();
	bool getAll();
	bool getBoundCheck();

};
#endif
