
#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>

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

    //data
    string ID;
    string xCoord;
    string yCoord;


  public:
    Parser();
    void setUserParam();
    void readFile();
    int getWidth();
    int getHeight();
};
#endif