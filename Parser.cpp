#include "Parser.h"
#include "Mapper.h"

#include <iostream>

Parser::Parser()
{
    inFile = "";
    outFile = "";
    warehouseWidth = 0;
    warehouseHeight = 0;
    ans = "";
    mutator = 1;

    ID = "";
    xCoord = "";
    yCoord = "";
}

void Parser::setUserParam()
{   
    printf("What is the file name?\n");
    cin >> inFile;
    printf("Does this file need calibration? Y or N?\n");
    cin >> ans; 
    
    if(ans == "Y") {
        printf("What is the max warehouse width?\n");
        cin >> warehouseWidth;
        printf("What is the max warehouse height?\n");
        cin >> warehouseHeight;
        printf("By what factor should we scale the map?\n");
        cin >> mutator;
        
        warehouseHeight *= mutator;
        warehouseHeight *= mutator;

        outFile = inFile.substr(0,inFile.size()-4) + "_modified.csv";
        printf("A file with these parameters has been created as %s\n",outFile.c_str());
    } 
    else if(ans == "N")
        return;
    else {   
        printf("Please answer with Y or N after retyping the file name.\n");
        setUserParam();
    }
}
void Parser::readFile()
{
    if (ans == "Y")
    {
        ifstream myFile;
        ofstream newFile;

        myFile.open(inFile);
        newFile.open(outFile);
        if(myFile.is_open() && newFile.is_open()){
            
            getline(myFile,ID,',');
            while(myFile.good()&& newFile.good()){
                getline(myFile,xCoord,',');
                getline(myFile,yCoord,'\n');
                
                //change to stod
                int x = stoi(xCoord);
                int y = stoi(yCoord);
                //move to shelving platform of (0,0)
                if(x == 0)
                    x += 2; 
                if(y == 0)   
                    y += 1;

                newFile << ID << ',' << x * mutator << ',' << y * mutator << '\n';
                getline(myFile,ID,',');
            }
            myFile.close();
            newFile.close();
        }
        else  
            printf("Files could not be opened\n");
    } else{ //file is ready to be mapped as is
        ifstream myFile;
        myFile.open(inFile);

        Mapper grid = Mapper();

        if(myFile.is_open()){
            getline(myFile,ID,',');
            while(myFile.good()){
                getline(myFile,xCoord,',');
                getline(myFile,yCoord,'\n');
                getline(myFile,ID,',');
            }
            myFile.close();
        }
        else  
            printf("%s could not be opened\n",inFile.c_str());
    }
}

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}

