
#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>

using namespace std;

class Parser
{
  private:
    string filename;
    int warehouseWidth;
    int warehouseHeight;

  public:
    void readFile(string inFile, string outFile);
    void setParam();
};
#endif