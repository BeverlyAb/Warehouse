#ifndef PARSE_H
#define PARSE_H

#define STOCK 0
#define NAME_ITEM 1
#define ORDER_FILE 2
#define BB 3
#define QUAD 4

#include "Layout.h"

class Parse
{
    private:
        string inFile;
        unsigned int width, height;
        int execute, orderIndx;
        Layout grid;

        queue<unsigned int> namedItems;
  	    queue<unsigned int> optItems;

    public:
        Parse();
        Parse(string, unsigned int, unsigned int);
        void setAction(int);
        int getAction();
        void readFile();
        void writeToFile(string);
        void getNameItem();
        void processOrder();
        void getOrder(string, int);
        void BB();
        void Quad();
};
#endif