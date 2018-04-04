
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
    unsigned int ID;
    double xCoord;
    double yCoord;

  public:
    Parser();
    void setUserParam();
    void readFile();
    string getAns();
};
#endif