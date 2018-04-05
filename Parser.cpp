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
        outFile = inFile.substr(0,inFile.size()-3) + "_modified.csv";
        printf("A file with these parameters has been created as %s\n",outFile.c_str());
    } else if(ans == "N")
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
           
            while(myFile.good()&& newFile.good()){
                getline(myFile,ID,'\t');
                getline(myFile,xCoord,'\t');
                getline(myFile,yCoord,'\n');
                newFile << stoi(ID.c_str()) << "\t"; 
                printf("%s %s %s\n", ID.c_str(), xCoord.c_str(), yCoord.c_str());

                     //   << (int)stod(xCoord) * mutator << "\t"  
                     //   << (int)stod(yCoord) * mutator << "\n"; 

            }
            myFile.close();
            newFile.close();
        }
        else  
            printf("Files could not be opened\n");
    } else{
        ifstream myFile;
        myFile.open(inFile);
        if(myFile.is_open()){

           while(myFile.good()){
               getline(myFile,ID,'\t');
               getline(myFile,xCoord,'\t');
               getline(myFile,yCoord,'\n');
                //for testing 
               printf("%s %s %s\n", ID.c_str(), xCoord.c_str(), yCoord.c_str());

            }
            myFile.close();
        }
        else  
            printf("%s could not be opened\n",inFile.c_str());
    }
}

