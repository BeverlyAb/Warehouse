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
	grid = Mapper();
}

void Parser::setUserParam()
{   
    printf("What is the file name?\n");
    cin >> inFile;

    printf("What is the max warehouse width?\n");
    cin >> warehouseWidth;
   	printf("What is the max warehouse height?\n");
    cin >> warehouseHeight;
    printf("By what factor should we scale the map?\n");
    cin >> mutator;
    warehouseHeight *= mutator;
    warehouseWidth *= mutator;
	
    grid = Mapper(warehouseWidth, warehouseHeight);
	
	printf("Does this file need calibration? Y or N?\n");
   	 	cin >> ans; 
	if(ans == "Y") {
        outFile = inFile.substr(0,inFile.size()-4) + "_modified.csv";
        printf("A file with these parameters has been created as %s\n",outFile.c_str());
    } 
    else if(ans == "N")
       outFile = inFile;
    else {   
        printf("Please answer with Y or N after retyping the file name.\n");
        setUserParam();
    }
}
void Parser::readFile(int fileType)
{
    ifstream myFile;
        
    myFile.open(outFile.c_str());

    if(myFile.is_open()){
        getline(myFile,ID,',');
        while(myFile.good()){
            getline(myFile,xCoord,',');
            getline(myFile,yCoord,'\n');
			if(fileType == STOCK){
            	grid.makeStock(atoi(ID.c_str()), atoi(xCoord.c_str()), atoi(yCoord.c_str()));
				
			}//else if(fileType == ORDER_LIST)
			//	grid,
			
            getline(myFile,ID,',');
        }
        myFile.close();
        
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int ID = 0;

        cout << "Starting x-coord?\n";
        cin >> x;
        cout << "Starting y-coord?\n";
        cin >> y;
        position start = {x,y};

        cout << "Product ID?\n";
        cin >> ID;
        
        if(grid.isValid(start)) {
            grid.nextPos(start, ID);
			printf("pos (%i,%i)\n",grid.getPos(ID).x,grid.getPos(ID).y);
		}
        else {
            printf("Starting position is either out of bounds or starts on a shelf.\n");
            return;
        }		
    } 
    else  
        printf("%s could not be opened\n",inFile.c_str());
    
}

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}
/*
void Parser::readOrder()
{
	printf("What is the file name?\n");
    cin >> inFile;
  	ifstream myFile;
        
    myFile.open(inFile.c_str());

    Mapper grid = Mapper(warehouseWidth, warehouseHeight);

    if(myFile.is_open()){
        getline(myFile,ID,',');
        while(myFile.good()){
            getline(myFile,xCoord,',');
            getline(myFile,yCoord,'\n');
            grid.makeMap(atoi(ID.c_str()), atoi(xCoord.c_str()), atoi(yCoord.c_str()));

            getline(myFile,ID,',');
        }
        myFile.close();
        
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int ID = 0;

        cout << "Starting x-coord?\n";
        cin >> x;
        cout << "Starting y-coord?\n";
        cin >> y;
        position start = {x,y};

        cout << "Product ID?\n";
        cin >> ID;
        
        if(grid.isValid(start)) 
            grid.nextPos(start, ID);
        else {
            printf("Starting position is either out of bounds or starts on a shelf.\n");
            return;
        }		
    }
    else  
        printf("%s could not be opened\n",inFile.c_str());
    
}*/

