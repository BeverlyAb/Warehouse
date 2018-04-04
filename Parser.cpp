#include "Parser.h"
#include <iostream>
Parser::Parser()
{
    inFile = "";
    outFile = "";
    warehouseWidth = 0;
    warehouseHeight = 0;
    ans = "";
    mutator = 1;

    ID = 0;
    xCoord = 0;
    yCoord = 0;
}

void Parser::setUserParam()
{   
    cout << "What is the file name?\n";
    cin >> inFile;
    cout << "Does this file need calibration? Y or N?\n";
    cin >> ans;
    
    if(ans == "Y") {
        cout << "What is the max warehouse width?\n";
        cin >> warehouseWidth;
        cout << "What is the max warehouse height\n";
        cin >> warehouseHeight;
        cout << "By what factor should we scale the map?\n";
        cin >> mutator;
        outFile = inFile + "_modified";
        cout << "A file with these parameters has been created as " + outFile << endl;
    } else if(ans == "N")
        return;
    else {   
        cout << "Please answer with Y or N after retyping the file name.\n";
        setUserParam();
    }
}
void Parser::readFile()
{
    ifstream myFile(inFile);
    ofstream newFile(outFile);
    if(myFile.is_open() && newFile.is_open()){
        while(myFile.good() && !myFile.eof()){
            cin >> ID;
            cin >> xCoord;
            cin >> yCoord;
            newFile << ID << "\t" 
                    << int(xCoord) * mutator << "\t"
                    << int(yCoord) * mutator << "\n";
        }
        myFile.close();
        newFile.close();
    }
    else  
        cout << "Files could not be opened\n";
}

string Parser::getAns()
{
    return ans;
}
